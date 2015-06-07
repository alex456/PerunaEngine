//
//  Geom.cpp
//  GLFW_Demo
//
//  Created by Ira on 6/3/15.
//  Copyright (c) 2015 C3. All rights reserved.
//

#include "Geom.h"


Geom::Geom(){
	init();
}


void Geom::init() {
	void calcVerts();
	void calcInds(); 
	createBuffers();
	calcFaces();
	calcPrimitives();
}

void Geom::createBuffers(){
	
	// 1. Create and bind VAO
	glGenVertexArrays(1, &vaoID); // Create VAO
	glBindVertexArray(vaoID); // Bind VAO (making it active)
	//2. Create and bind VBO
	// a. Vertex attributes
	glGenBuffers(1, &vboID); // Create VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind vertex attributes VBO
	int vertsDataSize = sizeof (float)* static_cast<int>(interleavedPrims.size());
	glBufferData(GL_ARRAY_BUFFER, vertsDataSize, NULL, GL_STREAM_DRAW); // allocate space
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertsDataSize, &interleavedPrims[0]); // upload the data

	// b. Indices  uses ELEMENT_ARRAY_BUFFER
	glGenBuffers(1, &indexVboID); // Generate buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboID); // Bind indices VBO
	int indsDataSize = static_cast<int>(inds.size()) * 3 * sizeof (GL_UNSIGNED_INT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsDataSize, NULL, GL_STATIC_DRAW); // allocate
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indsDataSize, &indPrims[0]); // upload the data

	for (int i = 0; i < 2; i++) {
		glEnableVertexAttribArray(i);
	}
	// STRIDE is 15: pos(3) + norm(3) + col(4) + uv(2) + tang(3)
	// (x, y, z, nx, ny, nz, r, g, b, a, u, v, tx, ty, tz)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof (GLfloat), BUFFER_OFFSET(0)); // pos
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, STRIDE * sizeof (GLfloat), BUFFER_OFFSET(12)); // col

	// Disable VAO
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

void Geom::calcFaces() {
    for(int i=0; i<inds.size(); ++i){
        faces.push_back( Face(verts.at(inds.at(i).e0), verts.at(inds.at(i).e1), verts.at(inds.at(i).e2)) );
    }                                                
}

void Geom::calcPrimitives(){
		if (interleavedPrims.size()>0) interleavedPrims.clear();
		if (indPrims.size()>0) indPrims.clear();

		for (int i = 0; i < verts.size(); i++) {
			// fill interleaved primitive arrays
			interleavedPrims.push_back(verts.at(i).x);
			interleavedPrims.push_back(verts.at(i).y);
			interleavedPrims.push_back(verts.at(i).z);

			interleavedPrims.push_back(.5);
			interleavedPrims.push_back(.5);
			interleavedPrims.push_back(1.0);
			interleavedPrims.push_back(1.0);
		// explode inds arrays to primitives
		for (int i = 0, j = 0; i < inds.size(); i++) {
			indPrims.push_back(inds.at(i).e0);
			indPrims.push_back(inds.at(i).e1);
			indPrims.push_back(inds.at(i).e2);
		}
	}
}

void Geom::display() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, static_cast<int>(inds.size()) * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);
}