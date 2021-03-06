// #include <iostream>
// #include "edge.h"
// #include "utils.h"
// #include "vertex.h"
// #include "triangle.h"
// #include "argparser.h"
// #include <mesh.h>
//
// int Triangle::next_triangle_id = 0;
//
// // =======================================================================
// // MESH DESTRUCTOR
// // =======================================================================
//
// // =======================================================================
// // the load function parses very simple .obj files
// // =======================================================================
//
// void Mesh::Load() {
//
//
//   std::string input_file = args->path + "/" + args->input_file;
//   std::cout<<input_file<<std::endl;
//
//
//   FILE *objfile = fopen(input_file.c_str(),"r");
//   if (objfile == NULL) {
//     std::cout << "ERROR! CANNOT OPEN '" << input_file << "'\n";
//     return;
//   }
//
//   char line[200] = "";
//   char token[100] = "";
//   char atoken[100] = "";
//   char btoken[100] = "";
//   char ctoken[100] = "";
//   float x,y,z;
//   int a,b,c,d,e;
//
//   int index = 0;
//   int vert_count = 0;
//   int vert_index = 1;
//
//   while (fgets(line, 200, objfile)) {
//     int token_count = sscanf (line, "%s\n",token);
//     if (token_count == -1) continue;
//     a = b = c = d = e = -1;
//     if (!strcmp(token,"usemtl") ||
// 	!strcmp(token,"g")) {
//       vert_index = 1;
//       index++;
//     } else if (!strcmp(token,"v")) {
//       vert_count++;
//       sscanf (line, "%s %f %f %f\n",token,&x,&y,&z);
//       addVertex(glm::vec3(x,y,z));
//     } else if (!strcmp(token,"f")) {
//       int num = sscanf (line, "%s %s %s %s\n",token,
// 			atoken,btoken,ctoken);
//       sscanf (atoken,"%d",&a);
//       sscanf (btoken,"%d",&b);
//       sscanf (ctoken,"%d",&c);
//       assert (num == 4);
//       a -= vert_index;
//       b -= vert_index;
//       c -= vert_index;
//       assert (a >= 0 && a < numVertices());
//       assert (b >= 0 && b < numVertices());
//       assert (c >= 0 && c < numVertices());
//       addTriangle(getVertex(a),getVertex(b),getVertex(c));
//     } else if (!strcmp(token,"vt")) {
//     } else if (!strcmp(token,"vn")) {
//     } else if (token[0] == '#') {
//     } else {
//       printf ("LINE: '%s'",line);
//     }
//   }
//
//   ComputeGouraudNormals();
//
//   std::cout << "loaded " << numTriangles() << " triangles " << std::endl;
// }
//
//
//
//
//
//
//
//
// Mesh::~Mesh() {
//   // delete all the triangles
//   std::vector<Triangle*> todo;
//   for (triangleshashtype::iterator iter = triangles.begin();
//        iter != triangles.end(); iter++) {
//     Triangle *t = iter->second;
//     todo.push_back(t);
//   }
//   int num_triangles = todo.size();
//   for (int i = 0; i < num_triangles; i++) {
//     removeTriangle(todo[i]);
//   }
//   // delete all the vertices
//   int num_vertices = numVertices();
//   for (int i = 0; i < num_vertices; i++) {
//     delete vertices[i];
//   }
//   cleanupVBOs();
// }
//
//
// // =======================================================================
// // MODIFIERS:   ADD & REMOVE
// // =======================================================================
//
// Vertex* Mesh::addVertex(const glm::vec3 &position) {
//   int index = numVertices();
//   Vertex *v = new Vertex(index, position);
//   vertices.push_back(v);
//   if (numVertices() == 1)
//     bbox = BoundingBox(position,position);
//   else
//     bbox.Extend(position);
//   return v;
// }
//
//
// void Mesh::addTriangle(Vertex *a, Vertex *b, Vertex *c) {
//   // create the triangle
//   Triangle *t = new Triangle();
//   // create the edges
//   Edge *ea = new Edge(a,b,t);
//   Edge *eb = new Edge(b,c,t);
//   Edge *ec = new Edge(c,a,t);
//   // point the triangle to one of its edges
//   t->setEdge(ea);
//   // connect the edges to each other
//   ea->setNext(eb);
//   eb->setNext(ec);
//   ec->setNext(ea);
//   // verify these edges aren't already in the mesh
//   // (which would be a bug, or a non-manifold mesh)
//   assert (edges.find(std::make_pair(a,b)) == edges.end());
//   assert (edges.find(std::make_pair(b,c)) == edges.end());
//   assert (edges.find(std::make_pair(c,a)) == edges.end());
//   // add the edges to the master list
//   edges[std::make_pair(a,b)] = ea;
//   edges[std::make_pair(b,c)] = eb;
//   edges[std::make_pair(c,a)] = ec;
//   // connect up with opposite edges (if they exist)
//   edgeshashtype::iterator ea_op = edges.find(std::make_pair(b,a));
//   edgeshashtype::iterator eb_op = edges.find(std::make_pair(c,b));
//   edgeshashtype::iterator ec_op = edges.find(std::make_pair(a,c));
//   if (ea_op != edges.end()) { ea_op->second->setOpposite(ea); }
//   if (eb_op != edges.end()) { eb_op->second->setOpposite(eb); }
//   if (ec_op != edges.end()) { ec_op->second->setOpposite(ec); }
//   // add the triangle to the master list
//   assert (triangles.find(t->getID()) == triangles.end());
//   triangles[t->getID()] = t;
// }
//
//
// void Mesh::removeTriangle(Triangle *t) {
//   Edge *ea = t->getEdge();
//   Edge *eb = ea->getNext();
//   Edge *ec = eb->getNext();
//   Vertex *a = ea->getStartVertex();
//   Vertex *b = eb->getStartVertex();
//   Vertex *c = ec->getStartVertex();
//   // remove these elements from master lists
//   edges.erase(std::make_pair(a,b));
//   edges.erase(std::make_pair(b,c));
//   edges.erase(std::make_pair(c,a));
//   triangles.erase(t->getID());
//   // clean up memory
//   delete ea;
//   delete eb;
//   delete ec;
//   delete t;
// }
//
//
// // Helper function for accessing data in the hash table
// Edge* Mesh::getMeshEdge(Vertex *a, Vertex *b) const {
//   edgeshashtype::const_iterator iter = edges.find(std::make_pair(a,b));
//   if (iter == edges.end()) return NULL;
//   return iter->second;
// }
//
//
//
// // =======================================================================
//
// // compute the gouraud normals of all vertices of the mesh and store at each vertex
// void Mesh::ComputeGouraudNormals() {
//   int i;
//   // clear the normals
//   for (i = 0; i < numVertices(); i++) {
//     getVertex(i)->clearGouraudNormal();
//   }
//   // loop through all the triangles incrementing the normal at each vertex
//   for (triangleshashtype::iterator iter = triangles.begin();
//        iter != triangles.end(); iter++) {
//     Triangle *t = iter->second;
//     glm::vec3 n = ComputeNormal((*t)[0]->getPos(),
//                                 (*t)[1]->getPos(),
//                                 (*t)[2]->getPos());
//     (*t)[0]->incrGouraudNormal(n);
//     (*t)[1]->incrGouraudNormal(n);
//     (*t)[2]->incrGouraudNormal(n);
//   }
//   // finally, normalize the sum at each vertex
//   for (i = 0; i < numVertices(); i++) {
//     getVertex(i)->normalizeGouraudNormal();
//   }
// }
//
// // =================================================================
//
// glm::vec4 floor_color(0.9,0.8,0.7,1);
// glm::vec4 mesh_color(0.8,0.8,0.8,1);
// glm::vec4 mirror_color(0.1,0.1,0.2,1);
// glm::vec4 mirror_tint(0.85,0.9,0.95,1);
//
// glm::vec4 red(1.0,0,0,1);
// glm::vec4 green(0,1,0,0.5);
//
// float floor_factor = 0.75;
//
// // =======================================================================
// // =======================================================================
//
//
// // the light position can be animated
// glm::vec3 Mesh::LightPosition() const {
//   return glm::vec3(0,300,0);
// }
//
// void Mesh::initializeVBOs() {
//   glGenBuffers(1,&mesh_tri_verts_VBO);
//   glGenBuffers(1,&mesh_tri_indices_VBO);
//   // bbox.initializeVBOs();
// }
//
// void Mesh::cleanupVBOs() {
//   glDeleteBuffers(1,&mesh_tri_verts_VBO);
//   glDeleteBuffers(1,&mesh_tri_indices_VBO);
//   // bbox.cleanupVBOs();
// }
//
// void Mesh::SetupMesh() {
//   for (triangleshashtype::iterator iter = triangles.begin();
//        iter != triangles.end(); iter++) {
//     Triangle *t = iter->second;
//     glm::vec3 a = (*t)[0]->getPos();
//     glm::vec3 b = (*t)[1]->getPos();
//     glm::vec3 c = (*t)[2]->getPos();
//
//     glm::vec3 na = ComputeNormal(a,b,c);
//     glm::vec3 nb = na;
//     glm::vec3 nc = na;
//     if (args->gouraud_normals) {
//       na = (*t)[0]->getGouraudNormal();
//       nb = (*t)[1]->getGouraudNormal();
//       nc = (*t)[2]->getGouraudNormal();
//     }
//     int start = mesh_tri_verts.size();
//     mesh_tri_verts.push_back(VBOPosNormalColor(a,na,mesh_color));
//     mesh_tri_verts.push_back(VBOPosNormalColor(b,nb,mesh_color));
//     mesh_tri_verts.push_back(VBOPosNormalColor(c,nc,mesh_color));
//     mesh_tri_indices.push_back(VBOIndexedTri(start,start+1,start+2));
//   }
//
//   glBindBuffer(GL_ARRAY_BUFFER,mesh_tri_verts_VBO);
//   glBufferData(GL_ARRAY_BUFFER,
//         sizeof(VBOPosNormalColor) * mesh_tri_verts.size(),
//         &mesh_tri_verts[0],
//         GL_STATIC_DRAW);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_tri_indices_VBO);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//         sizeof(VBOIndexedTri) * mesh_tri_indices.size(),
//         &mesh_tri_indices[0], GL_STATIC_DRAW);
// }
//
//
// void Mesh::DrawMesh() {
//   HandleGLError("enter draw mesh");
//   glBindBuffer(GL_ARRAY_BUFFER,mesh_tri_verts_VBO);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_tri_indices_VBO);
//   glEnableVertexAttribArray(0);
//   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VBOPosNormalColor),(void*)0);
//   glEnableVertexAttribArray(1);
//   glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VBOPosNormalColor),(void*)sizeof(glm::vec3) );
//   glEnableVertexAttribArray(2);
//   glVertexAttribPointer(2, 3, GL_FLOAT,GL_FALSE,sizeof(VBOPosNormalColor), (void*)(sizeof(glm::vec3)*2));
//   glDrawElements(GL_TRIANGLES, mesh_tri_indices.size()*3,GL_UNSIGNED_INT, 0);
//   glDisableVertexAttribArray(0);
//   glDisableVertexAttribArray(1);
//   glDisableVertexAttribArray(2);
//   HandleGLError("leaving draw mesh");
// }
//
//
// // ======================================================================================
// // ======================================================================================
//
// void Mesh::setupVBOs() {
//   // delete all the old geometry
//   mesh_tri_verts.clear();
//   mesh_tri_indices.clear();
//
//   // setup the new geometry
//   SetupMesh();
//   // bbox.setupVBOs();
// }
//
// void Mesh::drawVBOs() {
//   // mode 1: STANDARD PHONG LIGHTING (LIGHT ON)
//   glUniform1i(GLCanvas::colormodeID, 1);
//   // shader 0: NO SHADER
//   glUniform1i(GLCanvas::whichshaderID, 0);
//
//
//   HandleGLError("enter draw vbos");
//   if (!args->mirror && !args->shadow) {
//     if (args->geometry) {
//       // glUniform1i(GLCanvas::whichshaderID, args->whichshader);
//       DrawMesh();
//       // glUniform1i(GLCanvas::whichshaderID, 0);
//     }
//   }
//
//
//   // mode 0: NO LIGHTING
//   glUniform1i(GLCanvas::colormodeID, 0);
//   //
//   // // DrawLight();
//   // if (args->bounding_box) {
//   //   bbox.drawVBOs();
//   // }
//
//
//   HandleGLError();
// }
//
// // =================================================================
