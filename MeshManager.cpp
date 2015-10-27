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
		object->textureCoord(0, 0);
		object->colour(clr0);

		object->position(v1);
		object->normal(n0);
		object->textureCoord(1, 1);
		object->colour(clr1);

		object->position(v2);
		object->normal(n0);
		object->textureCoord(1, 1);
		object->colour(clr2);

		object->position(v3);
		object->normal(n0);
		object->textureCoord(0, 1);
		object->colour(clr3);

		object->position(v1);
		object->normal(n1);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v5);
		object->normal(n1);
		object->textureCoord(1, 0);
		object->colour(clr5);

		object->position(v6);
		object->normal(n1);
		object->textureCoord(1, 1);
		object->colour(clr6);

		object->position(v2);
		object->normal(n1);
		object->textureCoord(0, 1);
		object->colour(clr2);

		object->position(v5);
		object->normal(n2);
		object->textureCoord(0, 0);
		object->colour(clr5);

		object->position(v4);
		object->normal(n2);
		object->textureCoord(1, 0);
		object->colour(clr4);

		object->position(v7);
		object->normal(n2);
		object->textureCoord(1, 1);
		object->colour(clr7);

		object->position(v6);
		object->normal(n2);
		object->textureCoord(0, 1);
		object->colour(clr6);

		object->position(v4);
		object->normal(n3);
		object->textureCoord(0, 0);
		object->colour(clr4);

		object->position(v0);
		object->normal(n3);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v3);
		object->normal(n3);
		object->textureCoord(1, 1);
		object->colour(clr3);

		object->position(v7);
		object->normal(n3);
		object->textureCoord(0, 1);
		object->colour(clr7);

		object->position(v3);
		object->normal(n4);
		object->textureCoord(0, 0);
		object->colour(clr3);

		object->position(v2);
		object->normal(n4);
		object->textureCoord(1, 0);
		object->colour(clr2);

		object->position(v6);
		object->normal(n4);
		object->textureCoord(1, 1);
		object->colour(clr6);

		object->position(v7);
		object->normal(n4);
		object->textureCoord(0, 1);
		object->colour(clr7);

		object->position(v1);
		object->normal(n5);
		object->textureCoord(0, 0);
		object->colour(clr1);

		object->position(v0);
		object->normal(n5);
		object->textureCoord(1, 0);
		object->colour(clr0);

		object->position(v4);
		object->normal(n5);
		object->textureCoord(1, 1);
		object->colour(clr4);

		object->position(v5);
		object->normal(n5);
		object->textureCoord(0, 1);
		object->colour(clr5);

		for (int i = 0; i < 6; i++){
			object->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
			object->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
		}

		/* We finished the object */
		object->end();

		/* Convert triangle list to a mesh */
		Ogre::String mesh_name = "cube.mesh";
		object->convertToMesh(mesh_name);
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
				//object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, i*num_samples_phi + (j + 1), i*num_samples_phi + j);
				//object->triangle(((i + 1) % num_samples_theta)*num_samples_phi + j, ((i + 1) % num_samples_theta)*num_samples_phi + (j + 1), i*num_samples_phi + (j + 1));

				// Two triangles per quad
				object->triangle(i*num_samples_phi + j, i*num_samples_phi + (j + 1), ((i + 1) % num_samples_theta)*num_samples_phi + j);
				object->triangle(i*num_samples_phi + (j + 1), ((i + 1) % num_samples_theta)*num_samples_phi + (j + 1), ((i + 1) % num_samples_theta)*num_samples_phi + j);
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

	//void Mesh_Manager::createSquare(Ogre::SceneManager* scene_manager){
	//	Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

	//		/* Create the 3D object */
	//	Ogre::ManualObject* object = NULL;
	//	Ogre::String object_name = "Square";
	//	object = scene_manager->createManualObject(object_name);
	//	object->setDynamic(false);

	//	/* Create triangle list for the object */
	//	Ogre::String material_name = "ObjectMaterial";
	//	object->begin(material_name, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	//	Ogre::Vector3 v0(-1.0,  1.0,  0.0);
	//	Ogre::Vector3 v1( 1.0,  1.0,  0.0);
	//	Ogre::Vector3 v2(-1.0, -1.0,  0.0);
	//	Ogre::Vector3 v3( 1.0, -1.0,  0.0);

	//	Ogre::Vector3 vertex_normal(0.0, 0.0, -1.0);

	//	Ogre::ColourValue vertex_colour(0.0, 0.0, 1.0);

	//	object->position(v0);
	//	object->normal(vertex_normal);
	//	object->textureCoord(0, 1);
	//	object->colour(vertex_colour);
	//	object->index(0);

	//	object->position(v1);
	//	object->normal(vertex_normal);
	//	object->textureCoord(1, 1);
	//	object->colour(vertex_colour);
	//	object->index(1);

	//	object->position(v2);
	//	object->normal(vertex_normal);
	//	object->textureCoord(0, 0);
	//	object->colour(vertex_colour);
	//	object->index(2);

	//	object->position(v3);
	//	object->normal(vertex_normal);
	//	object->textureCoord(1, 0);
	//	object->colour(vertex_colour);
	//	object->index(3);
	//	

	//	object->triangle(0, 1, 2);
	//	object->triangle(1, 3, 2);



	//	/* We finished the object */
	//	object->end();

	//	/* Convert triangle list to a mesh */
	//	Ogre::String mesh_name = "Square.mesh";
	//	object->convertToMesh(mesh_name);

	//};

	// Builds All Of Our Meshes When Called
	void Mesh_Manager::buildAllMeshes(Ogre::SceneManager* manager) {
		createCubeMesh(manager);
		createIcohedron(manager);
		createSphere(manager);
		createReverseSphere(manager);
		createSquare(manager);
	}
}
