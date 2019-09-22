
#define MAX_DISTANCE_UNITS 10
void mx_distance_unit_float(float _in, int _unit_from, int _unit_to,
                            float _convert_table[MAX_DISTANCE_UNITS],
                            float result) {
  float unit_ratio = (_convert_table[_unit_from] / _convert_table[_unit_to]);
  result = _in * unit_ratio;
}

void mx_distance_unit_vector2(vec2 _in, int _unit_from, int _unit_to,
                              float _convert_table[MAX_DISTANCE_UNITS],
                              out vec2 result) {
  float unit_ratio = (_convert_table[_unit_from] / _convert_table[_unit_to]);
  result = _in * unit_ratio;
}

void mx_distance_unit_vector3(vec3 _in, int _unit_from, int _unit_to,
                              float _convert_table[MAX_DISTANCE_UNITS],
                              out vec3 result) {
  float unit_ratio = (_convert_table[_unit_from] / _convert_table[_unit_to]);
  result = _in * unit_ratio;
}

void mx_distance_unit_vector4(vec4 _in, int _unit_from, int _unit_to,
                              float _convert_table[MAX_DISTANCE_UNITS],
                              out vec4 result) {
  float unit_ratio = (_convert_table[_unit_from] / _convert_table[_unit_to]);
  result = _in * unit_ratio;
}