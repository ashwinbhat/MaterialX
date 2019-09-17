#define E_UNIT_DIMENSIONLESS 0
#define E_UNIT_MM            1
#define E_UNIT_CM            2
#define E_UNIT_M             3
#define E_UNIT_KM            4
#define E_UNIT_INCH          5
#define E_UNIT_FOOT          6
#define E_UNIT_MILE          7

void mx_distance_unit_float(float _in , vec2 _unit, out float result) {
    float unitScale[8];
    unitScale[E_UNIT_DIMENSIONLESS] = 1.0;
    unitScale[E_UNIT_MM]            = 1.0;
    unitScale[E_UNIT_CM]            = 10.0;
    unitScale[E_UNIT_M]             = 1000.0;
    unitScale[E_UNIT_KM]            = 1000000.0;
    unitScale[E_UNIT_INCH]          = 25.4;
    unitScale[E_UNIT_FOOT]          = 304.8;
    unitScale[E_UNIT_MILE]          = 1609344.0;

    //_unit.x (from) --> _unit.y (to)
    int from = int(_unit.x);
    int to = int(_unit.y);
    result = _in * (unitScale[from] / unitScale[to]);
}

