# 📦 AppImageBuilder
- Rename the binary file as `Egg-Drop-Saga`
- Copy the sounds folder and binary file and paste into `AppImageBuilder/Egg-Drop-Saga.AppDir/usr/bin`
- Open Terminal in this AppImageBuilder directory and run
```bash
chmod +x Egg-Drop-Saga.AppDir
```

```bash
chmod +x appappimagetool-x86_64.AppImage
```

```bash
./appimagetool-x86_64.AppImage Egg-Drop-Saga.AppDir
```

> For First building Run `builder.sh`
