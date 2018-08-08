luaposix and lunix only provide wrappers for the POSIX-standard [`fcntl(2)`](https://linux.die.net/man/2/fcntl)-based locking. On Linux, this does not interoperate with [`flock(2)`](https://linux.die.net/man/2/flock)-based locking, which means that Lua code cannot interoperate with software that uses it, including shell scripts using the [`flock(1)`](https://linux.die.net/man/1/flock) utility. This package provides a wrapper around the `flock(2)` system call, closing the gap.

# Requirements

- Tested with Lua 5.3, but should work on 5.1 or later
- A system with [`fileno(3)`](https://linux.die.net/man/3/fileno) and [`flock(2)`](https://linux.die.net/man/2/flock)

# Installation

Install through [LuaRocks](https://luarocks.org):

```sh
luarocks install luaflock
```

# Usage

This library provides a single function, normally called `flock`. It takes up to three parameters.

- The file handle to lock. Should have been opened with `io.open` with appropriate modes. (I recommend `"r"` for shared locks on a pre-existing file, and `"a+"` for other locks.)
- The lock type. If unspecified or nil, `"exclusive"` is used.
    - `"sh"`, `"shared"`, `"rd"`, `"read"`, or `"passive"`: Shared lock. (`LOCK_SH`)
    - `"ex"`, `"excl"`, `"exclusive"`, `"wr"`, `"write"`, or `"active"`: Exclusive lock. (`LOCK_EX`)
    - `"un"`, `"unlock"`, `"close"`, or `"release"`: Release any currently-held lock. (`LOCK_UN`).
- `true` if nonblocking mode (`LOCK_NB`) should be used. Default is to block indefinitely.

It returns:

- `true`: The operation succeeded.
- `nil, "would block"`: Nonblocking mode was requested and the call would have blocked.
- `nil, "<...some other error message...>"`: Another error occurred.

See the [`flock(2)` man page](https://linux.die.net/man/2/flock) for more information.

Example:

```lua
local flock = require "flock"
local f = io.open("some/file","a+")
assert(flock(f, "shared", true))
```

# Copying

This code is licensed under the zlib license. Basically, you can do what you like with it, except specifically take credit for writing it. See `LICENSE.md` for more information.

