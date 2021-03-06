#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glisy/gl.h>
#include <glisy/vao.h>
#include <glisy/buffer.h>

void
glisyVAOInit(GlisyVAO *vao) {
  if (!vao) return;
  vao->length = 0;
  vao->useElements = GL_FALSE;
  GLsizei attrsize = sizeof(GlisyVAOAttribute) * GLISY_MAX_VAO_ATTRIBS;
  memset(vao->attributes, 0, attrsize);
  glGenVertexArrays(1, &vao->handle);
}

void
glisyVAOBind(GlisyVAO *vao) {
  if (!vao) return;
  glBindVertexArray(vao->handle);
}

void
glisyVAOUnbind(GlisyVAO *vao) {
  if (!vao) return;
  glBindVertexArray(0);
}

void
glisyVAODispose(GlisyVAO *vao) {
  if (!vao) return;
  glDeleteVertexArrays(1, &vao->handle);
  vao->handle = 0;
  vao->length = 0;
  vao->useElements = GL_FALSE;
}

void
glisyVAOUpdate(GlisyVAO *vao, GlisyBuffer *elements) {
  if (!vao) return;

  // number of bound attributes for this VAO
  GLuint length = vao->length;

  // bind vao
  glisyVAOBind(vao);

  // bind elements if given
  if (elements) {
    glisyBufferBind(elements);
    vao->useElements = GL_TRUE;
  } else {
    vao->useElements = GL_FALSE;
  }

  // bind vao attributes
  for (int i = 0; i < length; ++i) {
    GlisyVAOAttribute *attr = &vao->attributes[i];

    // ensure .normalized is true or false
    if (GL_TRUE != attr->buffer.normalized) {
      attr->buffer.normalized = GL_FALSE;
    }

    // bind current vbo
    glBindBuffer(GL_ARRAY_BUFFER, vao->vbo[i]);

    // set vbo data from attribute
    glBufferData(GL_ARRAY_BUFFER,
                 attr->buffer.size,
                 attr->buffer.data,
                 attr->buffer.usage);

    // bind attribute to location
    glVertexAttribPointer(attr->location,
                          attr->buffer.dimension,
                          attr->buffer.type,
                          attr->buffer.normalized,
                          0, 0);

    // enable attribute at location
    glEnableVertexAttribArray(attr->location);
  }
}

GLuint
glisyVAOPush(GlisyVAO *vao, GlisyVAOAttribute *attr) {
  GLint pid;
  if (!vao) return GL_FALSE;
  if (!attr) return GL_FALSE;

  if (vao->length < GLISY_MAX_VAO_ATTRIBS) {
    glGetIntegerv(GL_CURRENT_PROGRAM, &pid);
    if (attr->name) {
      attr->location = glGetAttribLocation(pid, attr->name);
    }

    if (attr->location < 0) {
      attr->location = vao->length;
    }

    GLuint *vbo = (GLuint *) &vao->vbo[vao->length];
    vao->attributes[vao->length++] = *attr;

    // init vbo
    glGenBuffers(1, vbo);
  }

  return vao->length;
}

GLuint
glisyVAOPop(GlisyVAO *vao) {
  if (!vao) return GL_FALSE;
  //@TODO(jwerle): implement this
  return 0;
}

GLuint
glisyVAOSplice(GlisyVAO *vao, GLint start, GLuint count) {
  if (!vao) return GL_FALSE;
  //@TODO(jwerle): implement this
  return 0;
}

GLuint
glisyVAOSet(GlisyVAO *vao, GLuint location, GlisyVAOAttribute *attr) {
  if (!vao) return GL_FALSE;
  if (!attr) return GL_FALSE;
  GLuint max = GLISY_MAX_VAO_ATTRIBS;

  if (vao->length < max && location < max) {
    attr->location = location;
    vao->attributes[location] = *attr;
    vao->length = location < vao->length ? vao->length : location;
  }

  return vao->length;
}

GLuint
glisyVAORemove(GlisyVAO *vao, GLuint location) {
  if (!vao) return GL_FALSE;
  //@TODO(jwerle): implement GlisyVAO_remove
  return 0;
}

void
glisyVAOAttributeBind(GlisyVAOAttribute *attribute) {
  if (!attribute) return;
  GLfloat data[4];
  GLuint dimension = attribute->buffer.dimension;
  GLuint location = attribute->location;
  memcpy(&data, &attribute->buffer.data, sizeof(data));
  switch (dimension) {
  case 4:
    glVertexAttrib4f(location, data[0], data[1], data[2], data[3]);
    break;
  case 3:
    glVertexAttrib3f(location, data[0], data[1], data[2]);
    break;
  case 2:
    glVertexAttrib2f(location, data[0], data[1]);
    break;
  case 1:
    glVertexAttrib1f(location, data[0]);
    break;
  }
}
