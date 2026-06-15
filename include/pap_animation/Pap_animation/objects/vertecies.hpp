#ifndef VERTECIES_HPP
#define VERTECIES_HPP
#include <vector>
#include <Pap_animation/model/vertex.hpp>

void Setup_vaos();
void Setup_sphere_vertecies(int stack_count, int sector_count, float radius);

extern unsigned int cube_VAO;
extern unsigned int cube_VBO;
extern unsigned int cube_EBO;
extern unsigned int cube_indices_amount;

extern unsigned int sphere_VAO;
extern unsigned int sphere_VBO;
extern unsigned int sphere_EBO;
extern unsigned int sphere_indices_amount;

extern unsigned int text_VAO;
extern unsigned int text_VBO;

#endif //VERTECIES_HPP
