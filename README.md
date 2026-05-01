# Linear Allocator in C++

A high-performance arena allocator that achieves **~5× faster allocation** than `new`/`delete` by replacing heap bookkeeping with a single pointer increment.

---

## How it Works

Pre-allocates one large buffer upfront and hands out slices by bumping an offset forward. Freeing is `reset()` — one integer assignment that reclaims everything at once.

```
[AAAAAAAA][BBBBBBBBBBBB][CCCC][          free          ]
                              ^
                              offset (bump pointer)
```

No free-lists. No locks. No fragmentation. Just pointer arithmetic.

---

## API

```cpp
LinearAllocator arena(64 * 1024 * 1024); // allocate 64 MB upfront

T*     obj = arena.make<T>(args...);      // allocate + construct
T*     arr = arena.make_array<T>(count);  // allocate + construct array
void*  mem = arena.allocate(size, align); // raw allocation

size_t marker = arena.save();             // snapshot offset
arena.restore(marker);                    // rewind to snapshot, O(1)
arena.reset();                            // free everything, O(1)
```

---

## Benchmark

100,000 allocations of a 16-byte object, Release build, x86-64, MSVC, C++20.

| Method           | Time   | Speed          |
|------------------|--------|----------------|
| `new` / `delete` | ~2 ms  | 1×             |
| Linear Allocator | ~0.4 ms| **~5× faster** |

---

## Why it Matters for Games

| Advantage | Detail |
|---|---|
| **Per-frame allocations** | Allocate freely each frame, call `reset()` at frame end |
| **Cache locality** | Objects are packed contiguously — fewer cache misses |
| **No fragmentation** | One contiguous block, always |
| **No lock contention** | Zero synchronization overhead |
| **Predictable timing** | Always O(1) — critical for hard frame-time budgets |

---

## Usage Pattern

```cpp
LinearAllocator frameArena(64 * 1024 * 1024);

while (running)
{
    DrawCall*    draws     = frameArena.make_array<DrawCall>(visibleMeshes);
    ParticleJob* particles = frameArena.make_array<ParticleJob>(activeParticles);

    Update();
    Render();

    frameArena.reset(); // discard all, O(1)
}
```

---

## When to Use / Avoid

| Use | Avoid |
|---|---|
| Per-frame scratch data | Objects with independent lifetimes |
| Particles, physics contacts, AI pathfinding | Anything needing individual `delete` |
| Asset loading intermediate buffers | General-purpose heap replacement |

---

## Requirements

C++20 or later — no external dependencies.

```
/std:c++20    (MSVC)
-std=c++20    (GCC / Clang)
```

---

## License

MIT