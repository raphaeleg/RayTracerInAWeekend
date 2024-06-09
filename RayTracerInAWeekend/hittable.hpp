#pragma once

class material;

class hit_record {
public:
	point3 p{ 0 };
	vec3 normal{ 0 };
	float t = 0;
	bool front_face = false;
	shared_ptr<material> mat;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public: 
	virtual ~hittable() = default;
	virtual bool hit (const ray& r, interval ray_t, hit_record& rec) const = 0;
};