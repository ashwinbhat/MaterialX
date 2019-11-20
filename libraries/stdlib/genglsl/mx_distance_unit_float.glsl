void mx_distance_unit_float(float _in, int _unit_from, int _unit_to, out float result)
{
    result = _in * mx_distance_unit_ratio(_unit_from, _unit_to);
}
