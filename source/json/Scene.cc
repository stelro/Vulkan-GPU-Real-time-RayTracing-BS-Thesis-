/* =======================================================================
   $File: Scene.cc
   $Date: 22/2/2019	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "Scene.hh"


namespace ost {

  NLOHMANN_JSON_SERIALIZE_ENUM( Material, {
    { Material::LAMBERTIAN , "lambertian" },
    { Material::METAL , "metal" },
    { Material::DIELECTRIC, "dielectric" },
    { Material::EMITTER , "emitter" }
  })


  // custom type conversions from JSON parser
  void from_json(const ns::json& j, ost::Vec2& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
  }

  void from_json(const ns::json& j, ost::Vec3& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
  }

  void from_json(const ns::json& j, ost::Vec4& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
    j.at("w").get_to(vec.w);
  }

  void from_json(const ns::json& j, Sphere& s) {

    j.at("center").get_to(s.center);
    j.at("diffuse").get_to(s.diffuse);
    j.at("radius").get_to(s.radius);
    j.at("material").get_to(s.material);
    j.at("ref_index").get_to(s.ref_index);
    j.at("fuzz").get_to(s.fuzz);

  }

  void from_json(const ns::json& j, Plane& r) {

    j.at("normal").get_to(r.normal);
    j.at("distance").get_to(r.distance);
    j.at("diffuse").get_to(r.diffuse);
    j.at("material").get_to(r.material);
    j.at("ref_index").get_to(r.ref_index);
    j.at("fuzz").get_to(r.fuzz);
  }

  void from_json(const ns::json& j, Rectangle& r) {

    j.at("aabb").get_to(r.aabb);
    j.at("k").get_to(r.k);
    j.at("diffuse").get_to(r.diffuse);
    j.at("side").get_to(r.side);
    j.at("material").get_to(r.material);
    j.at("ref_index").get_to(r.ref_index);
    j.at("reverse_normals").get_to(r.reverse_normals);
    j.at("fuzz").get_to(r.fuzz);
  }

  void from_json(const ns::json& j, Box& b) {

    j.at("vmin").get_to(b.vmin);
    j.at("vmax").get_to(b.vmax);
    j.at("diffuse").get_to(b.diffuse);
    j.at("material").get_to(b.material);
    j.at("ref_index").get_to(b.ref_index);
    j.at("fuzz").get_to(b.fuzz);
  }

  Scene::Scene(const std::string &filename)  noexcept : m_objectId(0) {
    std::ifstream file(filename);
    OST_ASSERT_LOG(file.is_open(), "Failed to load JSON file, in Scene constructor!");
    m_jsonString = ns::json::parse(file);
    file.close();

    this->constructSpheres();
    this->constructPlanes();
    this->constructRectangles();
    this->constructBoxes();
  }

  std::vector<Sphere> Scene::getSpheres() const noexcept {

    return m_spheres;

  }

  std::vector<Plane> Scene::getPlanes() const noexcept {

    return m_planes;

  }

  std::vector<Rectangle> Scene::getRectangles()  const noexcept {

    return m_rectangles;
  }

  std::vector<Box> Scene::getBoxes() const noexcept {
    return m_boxes;
  }

  float Scene::getCameraFov() const noexcept {

    return m_jsonString["camera_fov"].get<float>();

  }

  std::array<float, 3> Scene::getCameraOrigin() const noexcept {

    return m_jsonString["camera_origin"].get<std::array<float,3>>();
  }

  std::array<float, 3> Scene::getCameraLookAt() const noexcept {

    return m_jsonString["camera_lookat"].get<std::array<float,3>>();
  }

  Scene::~Scene() noexcept {

    m_jsonString = nullptr;

  }

  u32 Scene::getWidth() const noexcept {

    return m_jsonString["width"].get<u32>();
  }

  u32 Scene::getHeight() const noexcept {

    return m_jsonString["height"].get<u32>();
  }

  void Scene::constructSpheres() noexcept {
    auto spheres_from_json = m_jsonString["spheres"];
    for ( const auto& sphere : spheres_from_json.items() ) {
      m_spheres.push_back( sphere.value().get<Sphere>() );
      m_spheres.back().object_id = m_objectId++;
    }
  }

  void Scene::constructPlanes() noexcept {
    auto rects_from_json = m_jsonString["planes"];
    for ( const auto& plane : rects_from_json.items() ) {
      m_planes.push_back( plane.value().get<Plane>() );
      m_planes.back().object_id = m_objectId++;
    }
  }

  void Scene::constructRectangles()  noexcept {
    auto rects_from_json = m_jsonString["rectangles"];
    for ( const auto& rect : rects_from_json.items() ) {
      m_rectangles.push_back( rect.value().get<Rectangle>() );
      m_rectangles.back().object_id = m_objectId++;
    }
  }

  void Scene::constructBoxes() noexcept {
    auto boxes_from_json = m_jsonString["boxes"];
    for ( const auto& box : boxes_from_json.items() ) {
      m_boxes.push_back( box.value().get<Box>() );
      m_boxes.back().object_id = m_objectId++;
    }
  }

  u32 Scene::getTotalObjects() const noexcept {
    return m_objectId;
  }

  ost::Vec3 Scene::getAmbientColor() const noexcept {
    return m_jsonString["ambient"].get<ost::Vec3>();
  }

  bool Scene::spheresAreEnabled() const noexcept {
    return m_jsonString["spheres_are_enabled"].get<bool>();
  }

  bool Scene::rectsAreEnabled() const noexcept {
    return m_jsonString["rectangles_are_enabled"].get<bool>();
  }

  bool Scene::planesAreEnabled() const noexcept {
    return m_jsonString["planes_are_enabled"].get<bool>();
  }

  bool Scene::boxesAreEnabled() const noexcept {
    return m_jsonString["boxes_are_enabled"].get<bool>();
  }

}