#include "MeshManager.h"

namespace Mesh_Manager_Space {

	// Creates Our Cube Mesh When Called
	void Mesh_Manager::createCubeMesh (Ogre::SceneManager* scene_manager) {
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		Ogre::String object_name = "Cube";
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);

		/* Create triangle list for the object */
		Ogre::String material_name = "ObjectMaterial";
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Vertices of a cube */
		Ogre::Vector3 v0(-0.5, -0.5,  0.5);
		Ogre::Vector3 v1( 0.5, -0.5,  0.5);
		Ogre::Vector3 v2( 0.5,  0.5,  0.5);
		Ogre::Vector3 v3(-0.5,  0.5,  0.5);
		Ogre::Vector3 v4(-0.5, -0.5, -0.5);
		Ogre::Vector3 v5( 0.5, -0.5, -0.5);
		Ogre::Vector3 v6( 0.5,  0.5, -0.5);
		Ogre::Vector3 v7(-0.5,  0.5, -0.5);

		/* Normal of each face of the cube */
		Ogre::Vector3 n0( 0.0,  0.0,  1.0);
		Ogre::Vector3 n1( 1.0,  0.0,  0.0);
		Ogre::Vector3 n2( 0.0,  0.0, -1.0);
		Ogre::Vector3 n3(-1.0,  0.0,  0.0);
		Ogre::Vector3 n4( 0.0,  1.0,  0.0);
		Ogre::Vector3 n5( 0.0, -1.0,  0.0);

		/* Cube's vertex colors */
		Ogre::ColourValue clr0(0.0, 0.0, 1.0);
		Ogre::ColourValue clr1(1.0, 0.0, 1.0);
		Ogre::ColourValue clr2(1.0, 1.0, 1.0);
		Ogre::ColourValue clr3(0.0, 1.0, 0.0);
		Ogre::ColourValue clr4(0.0, 0.0, 1.0);
		Ogre::ColourValue clr5(1.0, 0.0, 0.0);
		Ogre::ColourValue clr6(1.0, 1.0, 0.0);
		Ogre::ColourValue clr7(0.0, 1.0, 0.0);

		/* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
		to each face */
		object->position(v0);
		object->normal(n0);
		object->textureCoord(0.0, 0.0);
		object->colour(clr0);

		object->position(v1);
		object->normal(n0);
		object->textureCoord(0.2, 0.0);
		object->colour(clr1);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(0.2, 0.2);
		object->colour(clr2);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0.0, 0.2);
		object->colour(clr3);

		object->position(v1);
		object->normal(n1);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v5);
		object->normal(n1);
		object->textureCoord(0.2, 0.0);
		object->colour(clr5);

		object->position(v6);
		object->normal(n1);
		object->textureCoord(0.2, 0.2);
		object->colour(clr6);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0.0, 0.2);
		object->colour(clr2);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0.0, 0.0);
		object->colour(clr5);

		object->position(v4);
		object->normal(n2);
		object->textureCoord(0.2, 0.0);
		object->colour(clr4);

		object->position(v7);
		object->normal(n2);
		object->textureCoord(0.2, 0.2);
		object->colour(clr7);

		object->position(v6);
		object->normal(n2);
		object->textureCoord(0.0, 0.2);
		object->colour(clr6);

		object->position(v4);
		object->normal(n3);
		object->textureCoord(0.0, 0.0);
		object->colour(clr4);

		object->position(v0);
		object->normal(n3);
		object->textureCoord(0.2, 0.0);
		object->colour(clr0);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(0.2, 0.2);
		object->colour(clr3);

		object->position(v7);
		object->normal(n3);
		object->textureCoord(0.0, 0.2);
		object->colour(clr7);

		object->position(v3);
		object->normal(n4);
		object->textureCoord(0.0, 0.0);
		object->colour(clr3);

		object->position(v2);
		object->normal(n4);
		object->textureCoord(0.2, 0.0);
		object->colour(clr2);

		object->position(v6);
		object->normal(n4);
		object->textureCoord(0.2, 0.2);
		object->colour(clr6);

		object->position(v7);
		object->normal(n4);
		object->textureCoord(0.0, 0.2);
		object->colour(clr7);

		object->position(v1);
		object->normal(n5);
		object->textureCoord(0.0, 0.0);
		object->colour(clr1);

		object->position(v0);
		object->normal(n5);
		object->textureCoord(0.2, 0.0);
		object->colour(clr0);

		object->position(v4);
		object->normal(n5);
		object->textureCoord(0.2, 0.2);
		object->colour(clr4);

		object->position(v5);
		object->normal(n5);
		object->textureCoord(0.0, 0.2);
		object->colour(clr5);

		for (int i = 0; i < 6; i++){
			object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
			object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
		}

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		object->convertToMesh("cube.mesh");
	}


	void Mesh_Manager::createIcohedron (Ogre::SceneManager* scene_manager) {
		/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		Ogre::String object_name = "Icosahedron";
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);

		/* Create triangle list for the object */
		Ogre::String material_name = "";
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Vertices of an icosahedron */
#define X 6.525731112119133606
#define Z 6.850650808352039932

		static float vdata[12][3] = {
			{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
			{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
			{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}};

		/* Vertex colors */
		static float clr[12][3] = {
			{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
			{1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {0.6, 0.4, 0.2},
			{1.0, 0.2, 0.8}, {1.0, 0.4, 0.0}, {0.0, 0.6, 0.0}, {0.6, 0.6, 0.6}};

		/* Faces */
		static int tindices [20][3] = {
			{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4},	{1, 8, 4}, 
			{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2}, 
			{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11},	{6, 1, 0}, 
			{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}, 
		};

		/* Add vertices and faces */
		for (int i = 0; i < 12; i++){
			object->position(Ogre::Vector3(vdata[i][0], vdata[i][1], vdata[i][2]));
			object->normal(Ogre::Vector3(vdata[i][0], vdata[i][1], vdata[i][2]));
			object->colour(Ogre::ColourValue(clr[i][0], clr[i][1], clr[i][2]));
		}

		for (int i = 0; i < 20; i++){
			object->triangle(tindices[i][0], tindices[i][1], tindices[i][2]);
		}

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		Ogre::String mesh_name = "Icosahedron";
		object->convertToMesh(mesh_name);

	}

	void Mesh_Manager::createSphere (Ogre::SceneManager* scene_manager) {

		/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		object = scene_manager->createManualObject("Sphere");
		object->setDynamic(false);

		/* Create triangle list for the object */
		object->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Add vertices to the object */
		float theta, phi; // Angles for parametric equation
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;
		Ogre::Vector2 texture_coord;

		float radius = 0.6;
		int num_samples_theta = 90;
		int num_samples_phi = 45;

		for (int i = 0; i < num_samples_theta; i++){

			theta = Ogre::Math::TWO_PI*i/(num_samples_theta-1); // angle theta

			for (int j = 0; j < num_samples_phi; j++){

				phi = Ogre::Math::PI*j/(num_samples_phi-1); // angle phi

				/* Define position, normal and color of vertex */
				vertex_normal = Ogre::Vector3(cos(theta)*sin(phi), sin(theta)*sin(phi), -cos(phi));
				vertex_position = Ogre::Vector3(vertex_normal.x*radius, vertex_normal.y*radius, vertex_normal.z*radius);
				vertex_color = Ogre::ColourValue(0.0, 0.0, 1.0);
				texture_coord = Ogre::Vector2(((float)i)/((float)num_samples_theta), 1.0-((float)j)/((float)num_samples_phi));

				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
				object->textureCoord(texture_coord);
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_samples_theta; i++){
			for (int j = 0; j < (num_samples_phi-1); j++){

				// Two triangles per quad
				object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, i*num_samples_phi + (j + 1), i*num_samples_phi + j);
				object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, ((i + 1) % num_samples_theta)*num_samples_phi + (j + 1), i*num_samples_phi + (j + 1));
			}
		}

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		object->convertToMesh("sphere.mesh");
	}

	// Creates Our Reverse Triangle Sphere Mesh... Use When Y
	void Mesh_Manager::createReverseSphere (Ogre::SceneManager* scene_manager) {

		/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		object = scene_manager->createManualObject("ReverseSphere");
		object->setDynamic(false);

		/* Create triangle list for the object */
		object->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Add vertices to the object */
		float theta, phi; // Angles for parametric equation
		Ogre::Vector3 vertex_position;
		Ogre::Vector3 vertex_normal;
		Ogre::ColourValue vertex_color;
		Ogre::Vector2 texture_coord;

		float radius = 0.6;
		int num_samples_theta = 90;
		int num_samples_phi = 45;

		for (int i = 0; i < num_samples_theta; i++){

			theta = Ogre::Math::TWO_PI*i/(num_samples_theta-1); // angle theta

			for (int j = 0; j < num_samples_phi; j++){

				phi = Ogre::Math::PI*j/(num_samples_phi-1); // angle phi

				/* Define position, normal and color of vertex */
				vertex_normal = Ogre::Vector3(cos(theta)*sin(phi), sin(theta)*sin(phi), -cos(phi));
				vertex_position = Ogre::Vector3(vertex_normal.x*radius, vertex_normal.y*radius, vertex_normal.z*radius);
				vertex_color = Ogre::ColourValue(0.0, 0.0, 1.0);
				texture_coord = Ogre::Vector2(((float)i)/((float)num_samples_theta), 1.0-((float)j)/((float)num_samples_phi));

				/* Add them to the object */
				object->position(vertex_position);
				object->normal(vertex_normal);
				object->colour(vertex_color); 
				object->textureCoord(texture_coord);
			}
		}

		/* Add triangles to the object */
		for (int i = 0; i < num_samples_theta; i++){
			for (int j = 0; j < (num_samples_phi-1); j++){

				// Two triangles per quad
				object->triangle(i*num_samples_phi + j, i*num_samples_phi + (j + 1), ((i + 1) % num_samples_theta)*num_samples_phi + j);
				object->triangle(i*num_samples_phi + (j + 1), ((i + 1) % num_samples_theta)*num_samples_phi + (j + 1), ((i + 1) % num_samples_theta)*num_samples_phi + j);
			}
		}

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		object->convertToMesh("reverse.sphere.mesh");
	}

	void Mesh_Manager::createEnemy1(Ogre::SceneManager* scene_manager){

		/* Create a cube */

		/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		Ogre::String object_name = "Enemy1";
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);

		/* Create triangle list for the object */
		Ogre::String material_name = "ObjectMaterial";
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Vertices of a cube */
		Ogre::Vector3 v0( 0.5, -0.5,  0.0);
		Ogre::Vector3 v1( -0.5, -0.5,  0.0);
		Ogre::Vector3 v2( 0.0, -0.5, -2.0);
		Ogre::Vector3 v3( 0.0, 0.5, 0.0);
		Ogre::Vector3 v4( 0.5, -0.5, 0.0);  //copy of v0
		Ogre::Vector3 v5( -0.5, -0.5, 0.0);  //copy of v1



		/* Normal of each face of the cube */
		Ogre::Vector3 n0( 1.0,  1.0,  0.0);
		Ogre::Vector3 n1( -1.0,  1.0,  0.0);
		Ogre::Vector3 n2( 0.0,  -1.0,  0.0);
		Ogre::Vector3 n3( 0.0,  0.0,  1.0); //back

		/* Cube's color */
		Ogre::ColourValue clr0(1.0, 0.0, 0.0);
		Ogre::ColourValue clr1(1.0, 0.0, 1.0);
		Ogre::ColourValue clr2(1.0, 1.0, 1.0);
		Ogre::ColourValue clr3(0.0, 1.0, 0.0);
		Ogre::ColourValue clr4(0.0, 0.0, 1.0);


		/* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
		to each face */
		/* Each face of the cube is defined by four vertices (with the same normal) and two triangles */


	//left side
		object->position(v0);
		object->normal(n0);
		object->textureCoord(0,0);
		object->colour(clr4);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(0.2,0.0);
		object->colour(clr4);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0.0, 0.2);
		object->colour(clr4);

		object->position(v4);
		object->normal(n0);
		object->textureCoord(0,0);
		object->colour(clr1);

		object->position(v1);
		object->normal(n1);
		object->textureCoord(0.0,0.0);
		object->colour(clr4);

		object->position(v3);
		object->normal(n1);
		object->textureCoord(0.2,0.0);
		object->colour(clr3);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0.0, 0.2);
		object->colour(clr2);

		object->position(v5);
		object->normal(n1);
		object->textureCoord(0.0,0.0);
		object->colour(clr4);

		//bottom
		object->position(v1);
		object->normal(n2);
		object->textureCoord(0.0,0.0);
		object->colour(clr1);

		object->position(v0);
		object->normal(n2);
		object->textureCoord(0.2,0.0);
		object->colour(clr4);

		object->position(v2);
		object->normal(n2);
		object->textureCoord(0.0, 0.2);
		object->colour(clr0);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0.0,0.0 );
		object->colour(clr2);

		//back
		object->position(v0);
		object->normal(n3);
		object->textureCoord(0.0,0.0);
		object->colour(clr3);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(0.2, 0.0);
		object->colour(clr2);

		object->position(v1);
		object->normal(n3);
		object->textureCoord(0.0, 0.2);
		object->colour(clr1);

		object->position(v4);
		object->normal(n3);
		object->textureCoord(0.0, 0.0);
		object->colour(clr1);



		for (int i = 0; i < 2; i++){
			object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
			object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
		}



		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		Ogre::String mesh_name = "Enemy1";
		object->convertToMesh(mesh_name);



	}

	void Mesh_Manager::createCylinder(Ogre::SceneManager* scene_manager){

		/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		Ogre::ManualObject* object = NULL;
		Ogre::String object_name = "Cylinder";
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);
		Ogre::String material_name = "ObjectMaterial";



		//---Create a cylinder---//

		Ogre::Real origin = 0.0;
		Ogre::Real height = 5.0;
		Ogre::Real radius = 0.5;
		const int cylinder_circle_resolution = 120;
		int loop_count;
		Ogre::ColourValue cylinder_colour = Ogre::ColourValue(0.0, 0.2, 1.0);
		Ogre::Degree theata = Ogre::Degree(0);
		Ogre::Degree alpha = Ogre::Degree(360/cylinder_circle_resolution);
		Ogre::Vector3 cylinder_circle_center1;
		Ogre::Vector3 cylinder_circle_center2;
		Ogre::Vector3 cylinder_circle1[cylinder_circle_resolution];
		Ogre::Vector3 cylinder_circle2[cylinder_circle_resolution];

		cylinder_circle_center1.x = 0;
		cylinder_circle_center1.y = origin - height/2;
		cylinder_circle_center1.z = 0;

		cylinder_circle_center2.x = 0;
		cylinder_circle_center2.y = origin + height/2;
		cylinder_circle_center2.z = 0;

		for(int loop_count = 0; loop_count < cylinder_circle_resolution; loop_count++){
			theata = theata + alpha;
			cylinder_circle1[loop_count].x = Ogre::Math::Cos(theata) * radius;
			cylinder_circle1[loop_count].y = origin - height/2;
			cylinder_circle1[loop_count].z = Ogre::Math::Sin(theata) * radius;

			cylinder_circle2[loop_count].x = Ogre::Math::Cos(theata) * radius;
			cylinder_circle2[loop_count].y = origin + height/2;
			cylinder_circle2[loop_count].z = Ogre::Math::Sin
				(theata) * radius;
		}

		//---bottom circle---//
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_FAN);
		object->colour(Ogre::ColourValue(0.0, 0.0, 1.0));
		object->position(cylinder_circle_center1);

		for(loop_count = 0; loop_count < cylinder_circle_resolution; loop_count++){
			object->position(cylinder_circle1[loop_count]);
		}

		object->position(cylinder_circle1[0]);
		object-> end();

		//---body of cylinder---//
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);
		object->colour(Ogre::ColourValue(1.0, 0.0, 0.0));

		for(loop_count = 0; loop_count < cylinder_circle_resolution-1; loop_count++){
			object->position(cylinder_circle1[loop_count]);
			object->position(cylinder_circle2[loop_count]);
			object->position(cylinder_circle1[loop_count+1]);

			object->position(cylinder_circle1[loop_count+1]);
			object->position(cylinder_circle2[loop_count]);
			object->position(cylinder_circle2[loop_count+1]);
		}

		object->position(cylinder_circle1[loop_count]);
		object->position(cylinder_circle2[loop_count]);
		object->position(cylinder_circle1[0]);

		object->position(cylinder_circle1[0]);
		object->position(cylinder_circle2[loop_count]);
		object->position(cylinder_circle2[0]);

		object->end();

		//---top circle---//
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_FAN);
		object->colour(Ogre::ColourValue(0.0, 1.0, 0.0));
		object->position(cylinder_circle_center2);

		for(loop_count = cylinder_circle_resolution-1; loop_count > 0; --loop_count){
			object->position(cylinder_circle2[loop_count]);
		}

		object->position(cylinder_circle2[cylinder_circle_resolution-1]);

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		object->convertToMesh(object_name);



	}

	void Mesh_Manager::createPrism(Ogre::SceneManager* scene_manager){


		/* Create a cube */

		/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		Ogre::String object_name = "Prism";
		object = scene_manager->createManualObject(object_name);
		object->setDynamic(false);

		/* Create triangle list for the object */
		Ogre::String material_name = "ObjectMaterial";
		object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		/* Vertices of a cube */
		Ogre::Vector3 v0( 0.0, -0.5, 0.0);
		Ogre::Vector3 v1( -1.0, -0.5, 0.0);
		Ogre::Vector3 v2( -0.5, 0.5, 0.0);
		Ogre::Vector3 v3(  0.0, -0.5, 0.0);   //same as v1

		Ogre::Vector3 v4( 0.0, -0.5, 1.0);
		Ogre::Vector3 v5( -1.0,-0.5, 1.0);
		Ogre::Vector3 v6( -0.5, 0.5, 1.0);
		Ogre::Vector3 v7( 0.0, -0.5, 1.0);

		/* Normal of each face of the cube */
		Ogre::Vector3 n0( 0.0,  0.0,  -1.0);
		Ogre::Vector3 n1( 1.0,  1.0,  0.0);
		Ogre::Vector3 n2( -1.0,  1.0,  0.0);
		Ogre::Vector3 n3( 0.0,  -1.0,  0.0); //bottom
		Ogre::Vector3 n4( 0.0,  0.0,  1.0); //back

		/* Cube's color */
		Ogre::ColourValue clr0(1.0, 0.0, 0.0);
		Ogre::ColourValue clr1(1.0, 0.0, 1.0);
		Ogre::ColourValue clr2(1.0, 1.0, 1.0);
		Ogre::ColourValue clr3(0.0, 1.0, 0.0);
		Ogre::ColourValue clr4(0.0, 0.0, 1.0);


		/* This construction only partially uses shared vertices, so that we can assign appropriate vertex normals
		to each face */
		/* Each face of the cube is defined by four vertices (with the same normal) and two triangles */

		//front face
		object->position(v0);
		object->normal(n0);
		object->textureCoord(0.0, 0.0);
		object->colour(clr4);

		object->position(v1);
		object->normal(n0);
		object->textureCoord(0.2, 0.0);
		object->colour(clr4);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(0.0, 0.2);
		object->colour(clr2);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0.0, 0.0);
		object->colour(clr4);

		//left face
		object->position(v4);
		object->normal(n1);
		object->textureCoord(0.0, 0.0);
		object->colour(clr1);

		object->position(v3);
		object->normal(n1);
		object->textureCoord(0.2, 0.0);
		object->colour(clr4);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0.2, 0.2);
		object->colour(clr3);

		object->position(v6);
		object->normal(n1);
		object->textureCoord(0.0, 0.2);
		object->colour(clr2);

		//right side
		object->position(v1);
		object->normal(n2);
		object->textureCoord(0.0, 0.0);
		object->colour(clr4);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0.2, 0.0);
		object->colour(clr1);

		object->position(v6);
		object->normal(n2);
		object->textureCoord(0.2, 0.2);
		object->colour(clr4);

		object->position(v2);
		object->normal(n2);
		object->textureCoord(0.0, 0.2);
		object->colour(clr0);

		//Bottom
		object->position(v1);
		object->normal(n3);
		object->textureCoord(0.0, 0.0);
		object->colour(clr2);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(0.2, 0.0);
		object->colour(clr3);

		object->position(v7);
		object->normal(n3);
		object->textureCoord(0.2, 0.2);
		object->colour(clr2);

		object->position(v5);
		object->normal(n3);
		object->textureCoord(0.0, 0.2);
		object->colour(clr1);
		
		//back
		object->position(v4);
		object->normal(n4);
		object->textureCoord(0.0, 0.0);
		object->colour(clr2);

		object->position(v6);
		object->normal(n4);
		object->textureCoord(0.2, 0.3);
		object->colour(clr3);

		object->position(v5);
		object->normal(n4);
		object->textureCoord(0.2, 0.0);
		object->colour(clr2);

		object->position(v7);
		object->normal(n4);
		object->textureCoord(0.0, 0.0);
		object->colour(clr1);

		for (int i = 0; i < 5; i++){
			object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
			object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
		}



		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		Ogre::String mesh_name = "Prism";
		object->convertToMesh(mesh_name);

	}

	void Mesh_Manager::createExplosionParticleEffect(Ogre::SceneManager* scene_manager, int num_particles) {
				/* Retrieve scene manager and root scene node */
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		/* Create the 3D object */
		Ogre::ManualObject* object = NULL;
		object = scene_manager->createManualObject("particle");
		object->setDynamic(false);

		/* Create point list for the object */
		object->begin("", Ogre::RenderOperation::OT_POINT_LIST);

		/* Initialize random numbers */
		std::srand(std::time(0));

		/* Create a set of points which will be the particles */
		/* This is similar to drawing a sphere: we will sample points on a sphere, but will allow them to also
		deviate a bit from the sphere along the normal (change of radius) */
		float trad = 0.4; // Defines the starting point of the particles
		float maxspray = 0.5; // This is how much we allow the points to deviate from the sphere
		float u, v, w, theta, phi, spray; // Work variables
		for (int i = 0; i < num_particles; i++){

			// Randomly select three numbers to define a point in spherical coordinates
			u = ((double) rand() / (RAND_MAX));
			v = ((double) rand() / (RAND_MAX));
			w = ((double) rand() / (RAND_MAX));

			// Use u to define the angle theta along one direction of a sphere
			theta = u * 2.0 * 3.1416;
			// Use v to define the angle phi along the other direction of the sphere
			phi = acos(2.0*v - 1.0);
			// Use we to define how much we can deviate from the surface of the sphere (change of radius)
			spray = maxspray*pow((float) w, (float) (1.0/3.0)); // Cubic root of w

			// Define the normal and point based on theta, phi and the spray
			Ogre::Vector3 normal = Ogre::Vector3(spray*cos(theta)*sin(phi), spray*sin(theta)*sin(phi), spray*cos(phi));
			object->position(0.0, 0.0, 0.0);
			object->normal(normal);
			object->colour(Ogre::ColourValue(i/(float) num_particles, 0.0, 1.0 - (i/(float) num_particles))); // We can use the color for debug, if needed
		}

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		object->convertToMesh("particle.mesh");
	}

	// Builds All Of Our Meshes When Called
	void Mesh_Manager::buildAllMeshes(Ogre::SceneManager* manager) {
		createCubeMesh(manager);
		createIcohedron(manager);
		createSphere(manager);
		createReverseSphere(manager);
		createEnemy1(manager);
		createCylinder(manager);
		createPrism(manager);
		createExplosionParticleEffect(manager);
	}
}
