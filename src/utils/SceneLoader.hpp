#include <iostream>
#include <fstream>
#include <string>

#include "../God.hpp"
#include "../renderer/Mesh.hpp"
#include "../renderer/Camera.hpp"
#include "../renderer/Renderer.hpp"

#include "DataPath.hpp"

#include "glm/glm.hpp"

using namespace std;

class SceneLoader{
    public:
        static void load_scene(string filename){

            /* File Spec
             *
             * # Comment
             * Type(as String) @Speed
             * Position
             * Scale
             * Rotation
             */

            int l = 0;
            string name;
            Mesh* mesh;
            vec3 pos;
            vec3 scale;
            vec3 rot;

            string line;
            ifstream file(DataPath::get(filename));

            if(file.is_open()){
                while(getline(file, line)){
                    if(line.size() != 0  && line.at(0) != '#'){
                        
                        if(l == 0){
                            name = line;
                            mesh = _read_type(line);
                        }else if(l == 1){
                            pos   = _read_vec3(line);
                        }else if(l == 2){
                            scale = _read_vec3(line);
                        }else if(l == 3){
                            rot   = _read_vec3(line);

                            _create_entity(name, mesh, pos, scale, rot);
                        }

                        l++;
                        l = l % 4;
                    }
                }

                file.close();

            }else{ 
                cout << "Cannot open file " << filename << "\n";
            }
        }

        static void save_scene(string filename){

            /* File Spec
             *
             * # Comment
             * Type(as String) ? @Speed
             * Position
             * Scale
             * Rotation
             */

            ofstream file(DataPath::get(filename));

            if (file.is_open())
            {
                for(int i = 0; i < God::entities.capacity; i++){
                    Entity* e = God::entities[i];
                    if(e != nullptr){
                        file << e->name << "\n";
                        file << e->position.x << " " << e->position.y << " " << e->position.z << " \n";
                        file << e->scale.x << " " << e->scale.y << " " << e->scale.z << " \n";
                        file << e->rotation.x << " " << e->rotation.y << " " << e->rotation.z << " \n";
                    }
                }

                file.close();
            }
            else cout << "Unable to create file";
        }


    private:

        static Mesh* _read_type(string line){
            //@TODO(Kasper) be able to load builtin types and obj files
            if(line == "Cube"){
                return Mesh::get_cube();
            }else if(line == "Quad"){
                return Mesh::get_quad();
            }else if(line == "Sphere"){
                return Mesh::get_sphere();
            }

            return nullptr;
        }

        static vec3 _read_vec3(string line){
            vec3 v = vec3();
            int last = 0;
            int vi = 0;

            for(int i = 0; i < line.size(); i++){
                if(line.at(i) == ' '){
                    v[vi++] = atof(line.substr(last, i-last).c_str());
                    last = i;

                    if(vi == v.length() - 1){
                        v[vi++] = atof(line.substr(last, line.size()-last).c_str());
                        break;
                    }
                }
            }

            return v;
        } 

        static quat _read_quat(string line){
            quat v = quat();
            int last = 0;
            int vi = 0;

            for(int i = 0; i < line.size(); i++){
                if(line.at(i) == ' '){
                    v[vi++] = atof(line.substr(last, i-last).c_str());
                    last = i;
                    if(vi == v.length() - 1){
                        v[vi++] = atof(line.substr(last, line.size()-last).c_str());
                        break;
                    }
                }
            }

            return v;
        }

        static void _create_entity(string name, Mesh* mesh, vec3 pos, vec3 scale, vec3 rot){
            Entity* e;
            if(name == "Camera") 
                e = Renderer::instance->camera->entity; 
            else 
                e = new (God::entities.create()) Entity(); 

            e->name = name;
            e->mesh = mesh;
            e->position = pos;
            e->scale = scale;
            e->rotation = rot;
        }

};
