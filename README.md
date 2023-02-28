# TEngine

## State:
- [x] ECS
- [ ] RenderSystem:
  - [x] General interface
  - [x] OpenGL renderer
  - [ ] DirectX renderer
- [ ] Physics System:
  - [x] Basic movement
  - [ ] Advanced movement
  - [ ] Collisions
- [x] InputSystem
- [ ] Layers
- [ ] Editor

![state](https://res.cloudinary.com/dn4zzecop/image/upload/v1677610183/TEngine/1_o4sizu.jpg)

# Build
```
mkdir cmakebuild && cd cmakebuild
cmake -DAPI=OPENGL ..
cmake --build .
```