#include "RigidBody.h"

std::vector < glm::vec4> RigidBody::position = std::vector < glm::vec4>();
std::vector < glm::vec4> RigidBody::linearVelocity = std::vector < glm::vec4>();
std::vector < glm::vec4> RigidBody::angularVelocity = std::vector < glm::vec4>();
std::vector < glm::quat> RigidBody::rotation = std::vector < glm::quat>();
std::vector <GLint> RigidBody::partIndexes = std::vector<GLint>();