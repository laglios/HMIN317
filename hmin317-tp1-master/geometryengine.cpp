/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    //initCubeGeometry();
    //initCubeDeplierGeometry();
    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawCubeGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}
//! [2]
//!
void GeometryEngine::initCubeDeplierGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(0.0f, 0.0f,  0.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, 0.0f,  0.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(0.0f,  1.0f,  0.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  0.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( -1.0f, 0.0f,  0.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 0.0f, 0.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( -1.0f,  1.0f,  0.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 0.0f,  1.0f, 0.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, 0.0f, 0.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(2.0f, 0.0f, 0.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, 0.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(2.0f,  1.0f, 0.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(1.0f, 1.0f, 0.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(0.0f,  2.0f, 0.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(1.0f,  2.0f, 0.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(0.0f, -1.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f, 0.0f), QVector2D(0.66f, 0.5f) }, // v19
        {QVector3D(0.0f, 0.0f, 0.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, 0.0f, 0.0f), QVector2D(0.66f, 0.0f)}, // v17


        // Vertex data for face 5
        {QVector3D( 1.0f,  -1.0f, 0.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(0.0f,  -1.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  -2.0f, 0.0f), QVector2D(0.66f, 1.0f)},  // v23
        {QVector3D(0.0f,  -2.0f, 0.0f), QVector2D(0.33f, 1.0f)}, // v22
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
}

void GeometryEngine::initPlaneGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    int t1 = 4*16*16, t2=6*16*16;
    float s=0.25;
    std::cout << t1 << "-/-" << t2 << std::endl;
    VertexData vertices[1024];
    GLushort indices[1536];
    int pos=0,indice=0;

    float hauteur[17][17];

    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            hauteur[i][j]= rand()%2*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        }
    }

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            //std::cout << i << ":" << j << " " << pos*4+3 <<"/"<<t1-1 << std::endl;

            vertices[pos*4]={QVector3D(i*s-8*s, j*s-8*s, hauteur[i][j]), QVector2D(0.0f, 0.5f)};
            vertices[pos*4+1]={QVector3D(i*s+s-8*s, j*s-8*s,  hauteur[i+1][j]), QVector2D(0.33f, 0.5f)};
            vertices[pos*4+2]={QVector3D(i*s-8*s, j*s+s-8*s,  hauteur[i][j+1]), QVector2D(0.0f, 1.0f)};
            vertices[pos*4+3]={QVector3D(i*s+s-8*s, j*s+s-8*s, hauteur[i+1][j+1]), QVector2D(0.33f, 1.0f)};

            std::cout << i*s << "-" << j*s << std::endl;

            indices[6*pos]=indice;
            indices[6*pos+1]=indice;
            indices[6*pos+2]=indice+1;
            indices[6*pos+3]=indice+2;
            indices[6*pos+4]=indice+3;
            indices[6*pos+5]=indice+3;

            pos++;indice=indice+4;
        }
    }

    // index of the first strip needs to be duplicated.

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 1024* sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 1536 * sizeof(GLushort));
}


/*
void GeometryEngine::initPlaneGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    int t1 = 4*16*16, t2=6*16*16;
    float s=0.25;
    std::cout << t1 << "-/-" << t2 << std::endl;
    VertexData vertices[t1];
    GLushort indices[t2];
    int pos=0,indice=0;
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j=j+4){
            //std::cout << i << ":" << j << " " << pos*4+3 <<"/"<<t1-1 << std::endl;
            vertices[pos*4]={QVector3D(i-8, j,  -2+rand()%4), QVector2D(0.0f, 0.5f)};
            vertices[pos*4+1]={QVector3D(i-8, j-8+1,  -2+rand()%4), QVector2D(0.33f, 0.5f)};
            vertices[pos*4+2+16]={QVector3D(i-8, j-8+2,  -2+rand()%4), QVector2D(0.0f, 1.0f)};
            vertices[pos*4+3+17]={QVector3D(i-8, j-8+3,  -2+rand()%4), QVector2D(0.33f, 1.0f)};
            pos++;

            //std::cout << i << "-" << j << std::endl;


        }
    }
    pos =0;
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            indices[6*pos]=indice;
            indices[6*pos+1]=indice;
            indices[6*pos+2]=indice+1;
            indices[6*pos+3]=indice+16;
            indices[6*pos+4]=indice+17;
            indices[6*pos+5]=indice+17;

            pos++;indice=indice+1;
        }
    }

    for(int i=0;i<t2;i++)
        std::cout << "Indice : " << indices[i] << std::endl;

    // index of the first strip needs to be duplicated.

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, t1* sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, t2 * sizeof(GLushort));
}
*/

void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();



    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));


    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 6*16*16, GL_UNSIGNED_SHORT, 0);
}
