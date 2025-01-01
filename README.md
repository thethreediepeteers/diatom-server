# Dependencies

- Clang compiler infrastructure
- Binutils
- Mingw64 or WSL for Windows (Required!)

# Building 

## Docker

The most preferred way. Cross-platform and fast, however requires root.

It's the best solution if you're deploying diatom on a real server.

Use the Dockerfile:

```bash
docker build .
```

## Linux

You have 2 options of how to install the server. No root way is more preferred than root one.

--------

Non-root way (default)

```bash
./install-noroot.sh
```

The script will fetch dependencies and automatically build them.

Cleanup is done automatically 

Run `make` to compile or `RELEASE=1 make` to compile in release mode with optimizations.

--------

Root way (danger warning!)

```
./install.sh
```

This script will download dependencies and install them into global user include.

Beware! If you have older versions of uSockets or uWebSockets installed, they will be overwritten by latest unstable branch.

Cleanup is done automatically 

Run `make` to compile or `RELEASE=1 make` to compile in release mode with optimizations.

## Windows

The only way to build diatom server on windows is to download clang and g++ along with git and make.

If you use mingw git build, you will automatically get Mingw64.

You also must install binutils.

```bash
./install-noroot.sh
```

The script will fetch dependencies and automatically build them.

Cleanup is done automatically 

Run `make` to compile or `RELEASE=1 make` to compile in release mode with optimizations.

