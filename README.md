GlisyVAO
========

## Installation

```sh
$ clib install glisy/vao --save
```

## Usage

```c
#include <glisy/math.h>
#include <glisy/vao.h>

int
main (void) {
  const vec3 vertices[] = {
    vec3(-0.5, -0.5, +0.5),
    vec3(+0.5, -0.5, +0.5),
    vec3(-0.5, +0.5, +0.5),
    vec3(+0.5, +0.5, +0.5),

    vec3(-0.5, -0.5, -0.5),
    vec3(+0.5, -0.5, -0.5),
    vec3(-0.5, +0.5, -0.5),
    vec3(+0.5, +0.5, -0.5),
  };

  GlisyVAOAttribute vPosition;
  memset(&vPosition, 0, sizeof(vPosition));
  vPosition.buffer.data = (void *) vertices;
  vPosition.buffer.type = GL_FLOAT;
  vPosition.buffer.size = size;
  vPosition.buffer.usage = GL_STATIC_DRAW;
  vPosition.buffer.dimension = 3;

  GlisyVAO vao;
  glisyVAOInit(&vao);
  glisyVAOPush(&vao, &vPosition);
  glisyVAOBind(&vao);
  glisyVAOUpdate(&vao, 0);
  return 0;
}
```

## License

MIT
