# UWP build notes

- game code builds as `pk2_core`
- UWP starts from `ports/uwp/main.cpp`
- `res/` and the needed DLLs are copied into the package
- saves, settings, logs, and `mapstore/` live straight in `LocalState`

## vcpkg

Use your own vcpkg checkout, but run it from this repo root so it picks up `vcpkg.json` and `vcpkg-configuration.json`.

```powershell
cd C:\path\to\PekkaKana2
$env:VCPKG_ROOT = "C:\path\to\vcpkg"

& "$env:VCPKG_ROOT\vcpkg.exe" install --triplet x64-uwp
```

That installs this repo's deps into `vcpkg_installed/x64-uwp`.

The project carries a local SDL2 overlay here:

```text
ports/uwp/vcpkg-overlay/ports/sdl2
```

That is the SDL2 vcpkg should use for this build. Quick check:

```powershell
& "$env:VCPKG_ROOT\vcpkg.exe" install --triplet x64-uwp --dry-run
```

Look for:

```text
sdl2:x64-uwp@2.32.10#2 -- ...\ports\uwp\vcpkg-overlay\ports\sdl2
```

If you want to start the local deps from scratch:

```powershell
Remove-Item .\vcpkg_installed -Recurse -Force
& "$env:VCPKG_ROOT\vcpkg.exe" install --triplet x64-uwp
```

You should not need to remove any old classic/global SDL2 install. This build
uses the local `vcpkg_installed` folder.

## SDL2 HDMI patch

On Xbox, UWP can say the window is 1920x1080 even when the console is set to
1440p or 4K. The SDL2 overlay has `xbox-hdmi-size.patch`, which asks Xbox for
the real HDMI mode and lets SDL create the renderer output at that size.

vcpkg applies that patch when it builds SDL2.

## Configure

```powershell
cmake -S . -B build/uwp -G "Visual Studio 17 2022" -A x64 -DUWP_BUILD=ON
```

## Build

```powershell
cmake --build build/uwp --config Release --target PekkaKana2UWP
```

The package ends up here:

```text
build/uwp/ports/uwp/AppPackages/PekkaKana2UWP/
```

## Quick runtime check

Visual Studio output should show something like this on a 1440p Xbox:

```text
[PK2-UWP] SDL window 1920x1080, renderer output 2560x1440
```

The 1080p window bit is fine. Renderer output is the one that matters.
