
#define DISTANCE_UNITS 8
uniform float u_distance_unit_scales[DISTANCE_UNITS] = float[](
    1.0, 
    1.0,
    10.0,
    1000.0,
    1000000.0,
    25.4,
    304.8,
    1609344.0
);

float unit_ratio(vec2 unit) {
    //Decode _unit.x (from) --> _unit.y (to)
    int from = int(unit.x);
    int to = int(unit.y);
    return (u_distance_unit_scales[from] / u_distance_unit_scales[to]);
}

void mx_distance_unit_float(float _in , vec2 _unit, out float result) {
    result = _in * unit_ratio(_unit);
}

void mx_distance_unit_vector2(vec2 _in , vec2 _unit, out vec2 result) {
    result = _in * unit_ratio(_unit);
}

void mx_distance_unit_vector3(vec3 _in , vec2 _unit, out vec3 result) {
    result = _in * unit_ratio(_unit);
}

void mx_distance_unit_vector4(vec4 _in , vec2 _unit, out vec4 result) {
    result = _in * unit_ratio(_unit);
}