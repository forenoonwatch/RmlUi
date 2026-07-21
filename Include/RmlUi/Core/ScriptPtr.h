#pragma once

#include "RmlUi/Core/ObserverPtr.h"
#include <RmlUi/Core/Types.h>

namespace Rml {

template <typename T>
class ScriptPtr {
	public:
		static_assert(alignof(T) > 1 && alignof(Detail::ObserverPtrBlock) > 1, "ScriptPtr requires that alignof(T) and the ObserverPtrBlock must both be > 1");

		ScriptPtr() noexcept
			: ptr(nullptr)
			, owner(false) {}

		ScriptPtr(std::nullptr_t) noexcept
			: ptr(nullptr)
			, owner(false) {}

		ScriptPtr(ElementPtr&& element) noexcept
			: ptr(element.release())
			, owner(true) {}

		ScriptPtr(T* ptrIn) noexcept
				: ptr(nullptr)
				, owner(false) {
			if (ptrIn) {
				auto observer = ptrIn->GetObserverPtr();
				ptr = observer.block;
				observer.block = nullptr;
			}
		}

		~ScriptPtr() noexcept {
			reset();
		}

		// Move
		ScriptPtr& operator=(ScriptPtr&& other) noexcept {
			if (this != &other) {
				reset();
				ptr = std::exchange(other.ptr, nullptr);
				owner = std::exchange(other.owner, false);
			}

			return *this;
		}

		ScriptPtr(ScriptPtr&& other) noexcept {
			*this = std::move(other);
		}

		// Copy
		ScriptPtr(const ScriptPtr& other) noexcept {
			*this = other;
		}

		ScriptPtr& operator=(const ScriptPtr& other) noexcept {
			if (this != &other) {
				reset();
				owner = false;

				if (other.owner) {
					auto observer = other.value()->GetObserverPtr();
					ptr = observer.block;
					observer.block = nullptr;
				}
				else {
					ptr = other.block();
					block()->num_observers += 1;
				}
			}

			return *this;
		}

		// Returns true if we can dereference the pointer.
		explicit operator bool() const noexcept {
			return ptr && (owner || (!owner && block()->pointed_to_object));
		}

		T* get() const noexcept {
			if (ptr && owner) {
				return value();
			}
			else if (ptr) {
				return static_cast<T*>(block()->pointed_to_object);
			}
			else {
				return nullptr;
			}
		}

		T* operator->() const noexcept {
			return owner ? value() : static_cast<T*>(block()->pointed_to_object);
		}

		T& operator*() const noexcept {
			return owner ? *value() : *static_cast<T*>(block()->pointed_to_object);
		}

		// Comparison operators return true when they point to the same object, or they are both nullptr or expired.
		bool operator==(const T* other) const noexcept { return get() == other; }
		bool operator==(const ScriptPtr& other) const noexcept { return get() == other.get(); }

		void reset() noexcept {
			if (ptr && owner) {
				Releaser<T>{}(value());
			}
			else if (ptr) {
				block()->num_observers -= 1;
				Detail::DeallocateObserverPtrBlockIfEmpty(block());
			}

			ptr = nullptr;
		}
	private:
		void* ptr{};
		bool owner{};

		Detail::ObserverPtrBlock* block() const noexcept {
			return reinterpret_cast<Detail::ObserverPtrBlock*>(ptr);
		}

		T* value() const noexcept {
			return reinterpret_cast<T*>(ptr);
		}
};

}
