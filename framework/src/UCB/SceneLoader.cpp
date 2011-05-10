#include "SceneLoader.h"

#include "algebra3.h"

#include <fstream>
#include <string>

using namespace std;


SceneLoader::SceneLoader(Scene &scene, string file) {
    err = &cout;
    scene._root = new SceneInstance();
    root = scene._root;
    root->_name = "toplevel";
    buildScene(file);
}

SceneLoader::~SceneLoader() {
    for (map<string, SceneGroup*>::iterator it = groups.begin(); 
                                                 it != groups.end(); ++it)
    {
        delete it->second;
    }
    for (map<string, ParametricMaterial*>::iterator it = materials.begin(); 
                                                 it != materials.end(); ++it)
    {
        delete it->second;
    }
    for (vector<SceneInstance*>::iterator it = instances.begin(); it != instances.end(); ++it)
    {
        delete *it;
    }
}

void SceneLoader::buildEndlineTable(string filename)
{
    ifstream file(filename.c_str());
    string line;
    while (getline( file, line ))
        endlineTable.push_back(file.tellg());
    endlineTable.push_back(file.tellg());
}

void SceneLoader::curPos(ostream &out, int g)
{
    int line = 1;
    int lastg = 0;
    for (vector<int>::iterator it = endlineTable.begin(); it != endlineTable.end(); ++it)
    {
        if (*it > g) {
            out << "line: " << line << " char: " << (g - lastg - 1);
            return;
        }

        lastg = *it;
        line ++;
    }
    out << "line: " << line << " char: " << (g - lastg);
}

string SceneLoader::getString(istream &str)
{
    string ret;
    char temp;
    str >> temp; // get rid of white space, get to first char
    str.putback(temp);
    while (str.get(temp))
    {
        if (temp >= 'a' && temp <= 'z'
                || temp >= 'A' && temp <= 'Z'
                || temp >= '0' && temp <= '9'
                || temp == '_' )
        {
            ret += temp;
        }
        else if (temp == '#')
        {
            string no;
            getline(str, no);
            if (ret.empty())
                return getString(str);
            else
                return ret;
        }
        else {
            str.putback(temp);
            break;
        }
    }

    return ret;
}

string SceneLoader::getQuoted(istream &str)
{
    string ret;
    char temp;
    str >> temp;
    if (temp != '"') {
        *err << "expected opening \" at ";
        curPos(*err, str.tellg());
        *err << endl;
    }
    getline(str, ret, '"');
    return ret;
}

bool SceneLoader::readCommand(istream &str, string &name)
{
    name = getString(str);

    if (name.empty()) {
        *err << "error: expected command but did not find one at ";
        curPos(*err, str.tellg());
        *err << endl;
        return false;
    }

    return true;
}

bool SceneLoader::findOpenParen(istream &str)
{
    char temp;
    string line;
    while (str >> temp)
    {
        if (temp == '(')
            return true;
        else if (temp == '#')
            getline(str,line);
        else {
            *err << "error: unexpected char " << temp << " at ";
            curPos(*err, str.tellg());
            *err << endl;
            return false;
        }
    }
    return false;
}

enum { OPEN, CLOSED, ERROR };  // possible types of paren, used by the findOpenOrClosedParen function below
int SceneLoader::findOpenOrClosedParen(istream &str)
{
    char temp;
    string line;
    while (str >> temp)
    {
        if (temp == '(')
            return OPEN;
        else if (temp == '#')
            getline(str,line);
        else if (temp == ')') {
            str.putback(temp);
            return CLOSED;
        }
        else {
            *err << "error: unexpected char " << temp << " at ";
            curPos(*err, str.tellg());
            *err << endl;
            return false;
        }
    }
    return ERROR;
}

bool SceneLoader::findCloseParen(istream &str)
{
    char temp;
    string line;
    int close = 0;
    while (str >> temp)
    {
        if (temp == '#')
            getline(str, line);
        if (temp == '(')
            close++;
        if (temp == ')')
            close--;
        if (close < 0)
            return true;
    }
    return false;
}

int SceneLoader::getValues(istream &str, vector<ParametricValue*> &vals)
{
    ParametricValue *val;
    while ( (val = getValue(str)) != NULL )
    {
        vals.push_back(val);
    }
    return int(vals.size());
}

ParametricValue* SceneLoader::getValue(istream &str)
{
    char temp;
    str >> temp;
    if (temp == '{') // time-varying expression
    {
        int start = str.tellg();
        string expr;
        while (str.get(temp))
        {
            if (temp == '}') // completed expr
            {
                ParametricValue *v = new ExprValue(expr.c_str());
                if (!v->good()) {
                    *err << "Error: couldn't parse expression \"" << expr << "\" at ";
                    curPos(*err,str.tellg());
                    *err << endl;
                    delete v;
                    return NULL;
                } else {
                    return v;
                }
            }
            else if (temp == '#')
            {
                string no;
                getline(str, no);
            }
            else {
                expr += temp;
            }
        }
        *err << "Error: No closing brace for expr at ";
        curPos(*err, start);
        *err << endl;
        return NULL;
    }
    else if (temp == ')')
    {
        str.putback(temp);
        return NULL;
    }
    else if (temp == '#')
    {
        string no;
        getline(str,no);
        return getValue(str);
    }
    else
    {
        str.putback(temp);

        double val;
        str.peek();
        str >> val;
        if (str.fail())
        {
            str.clear();
            *err << "Failed to extract a numeric value at ";
            curPos(*err, str.tellg());
            *err << endl;

            str.peek();
            return NULL;
        } else {
            return new ConstValue(val);
        }
    }
}

void SceneLoader::errLine(int at)
{
    curPos(*err, at);
    *err << endl;
}

void SceneLoader::cleanAfter(vector<ParametricValue*> &vals, unsigned int i)
{
    for (;i<vals.size(); i++)
    {
        delete vals[i];
    }
}

SceneInstance* SceneLoader::doI(istream &str, string &name)
{
    name = getString(str);
    if (name.empty()) {
        *err << "Couldn't read instance name at "; errLine(str.tellg());
        return NULL;
    }

    string var = getString(str);
    if (groups[var] == NULL)
    {
        *err << "Instancing node " << var << " which doesn't exist yet at ";
        curPos(*err,str.tellg());
        *err << endl;
        return NULL;
    }

    SceneInstance *n = new SceneInstance();
    instances.push_back(n); //nodes[name] = n;
    n->_name = name;
    n->_child = groups[var];

    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR)
            return NULL;
        else if (state == CLOSED)
            return n;
        else if (state == OPEN)
        {
            string cmd;
            vector<ParametricValue*> values;
            if (readCommand(str, cmd)) {

                if (cmd == "R") // load rotations
                {
                    int numv = getValues(str, values);
                    Rotate *r = NULL;
                    if (numv < 1) {
                        *err << "R with no args at "; errLine(str.tellg());
                    } else if (numv < 4) {
                        cleanAfter(values, 1);
                        r = new Rotate();
                        r->angle = values[0];
                    } else {
                        cleanAfter(values, 4);
                        r = new Rotate();
                        r->angle = values[0];
                        for (int i = 0; i < 3; i++)
                            r->axis[i] = values[i+1];
                    }
                    if (r != NULL)
                        n->_transforms.push_back(r);
                }
                else if (cmd == "Xform")
                {
                    int numv = getValues(str, values);
                    GeneralTransform *g = NULL;
                    if (numv < 9) {
                        *err << "Xform with too few parameters at "; errLine(str.tellg());
                    } else if (numv < 16) { // 2d
                        cleanAfter(values, 9);
                        g = new GeneralTransform();
                        for (int i = 0; i < 9; i++)
                            g->matrix.push_back(values[i]);
                    } else {
                        cleanAfter(values, 16);
                        g = new GeneralTransform();
                        for (int i = 0; i < 16; i++)
                            g->matrix.push_back(values[i]);
                    }
                    if (g != NULL)
                        n->_transforms.push_back(g);
                }
                else if (cmd == "T")
                {
                    int numv = getValues(str, values);
                    Translate *t = NULL;
                    if (numv < 2) {
                        *err << "T with too few parameters at "; errLine(str.tellg());
                    } else if (numv == 2) {
                        t = new Translate();
                        for (int i = 0; i < 2; i++)
                            t->translate[i] = values[i];
                    } else {
                        cleanAfter(values, 3);
                        t = new Translate();
                        for (int i = 0; i < 3; i++)
                            t->translate[i] = values[i];
                    }
                    if (t != NULL)
                        n->_transforms.push_back(t);
                }
                else if (cmd == "S")
                {
                    int numv = getValues(str, values);
                    Scale *s = NULL;
                    if (numv < 2) {
                        *err << "S with too few parameters at "; errLine(str.tellg());
                    } else if (numv == 2) {
                        s = new Scale();
                        for (int i = 0; i < 2; i++)
                            s->scale[i] = values[i];
                    } else {
                        s = new Scale();
                        for (int i = 0; i < 3; i++)
                            s->scale[i] = values[i];
                    }
                    if (s != NULL)
                        n->_transforms.push_back(s);
                }
                else if (cmd == "color")
                {
                    int numv = getValues(str, values);
                    ParametricColor *c = NULL;
                    if (numv < 3) {
                        *err << "color with too few parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 3);
                        c = new ParametricColor();
                        for (int i = 0; i < 3; i++)
                            c->_color[i] = values[i];
                    }
                    if (c != NULL)
                        n->_color = c;
                }
                else if (cmd == "lod")
                {
                    int numv = getValues(str, values);
                    LOD *l = NULL;
                    if (numv < 1) {
                        *err << "lod with no parameters at "; errLine(str.tellg());
                    } else {
                        //cout << "got lod" << endl;
                        cleanAfter(values, 1);
                        l = new LOD();
                        l->_level = values[0];
                    }
                    if (l != NULL)
                        n->_lod = l;
                }
                else
                {
                    *err << "Error: command " << cmd << " not recognized at ";
                    curPos(*err,str.tellg());
                    *err << endl;
                }
                findCloseParen(str);
            }
        }
    } while (true);
}

bool SceneLoader::getName(istream &str, string type, string &name)
{
    name = getString(str);
    if (name.empty()) {
        *err << "Couldn't read " << type << " name at "; errLine(str.tellg());
        return false;
    }

    if (groups[name] != NULL)
    {
        *err << "Illegal re-use of name \"" << name << "\" at "; errLine(str.tellg());
        return false;
    }

    return true;
}

void SceneLoader::SetMaterialDefaults(ParametricMaterial *n)
{
    if (n->_vec3 == NULL) {
        n->_vec3 = new ParametricColor();
        n->_vec3->_color[0] = new ConstValue(0);
        n->_vec3->_color[1] = new ConstValue(0);
        n->_vec3->_color[2] = new ConstValue(0);
    }
    for (int i = 0; i < 7; i++)
    {
        if (n->_coefficients[i] == NULL)
            n->_coefficients[i] = new ConstValue(0);
    }
}

bool SceneLoader::doMaterial(istream &str, string &name)
{
    name = getString(str);
    if (name.empty()) {
        *err << "Couldn't read material name at "; errLine(str.tellg());
        return false;
    }

    if (materials[name] != NULL) {
        *err << "Illegal re-use of name \"" << name << "\" at "; errLine(str.tellg());
        return false;
    }

    map<string, int> indices;
    indices["ka"] = MAT_KA;
    indices["kd"] = MAT_KD;
    indices["ks"] = MAT_KS;
    indices["ksp"] = MAT_KSP;
    indices["ksm"] = MAT_KSM;
    indices["kt"] = MAT_KT;
    indices["ktn"] = MAT_KTN;


    ParametricMaterial *n = new ParametricMaterial();
    materials[name] = n;
    
    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR) {
            SetMaterialDefaults(n);
            return false;
        } else if (state == CLOSED) {
            SetMaterialDefaults(n);
            return true;
        }
        else if (state == OPEN)
        {
            string cmd;
            vector<ParametricValue*> values;
            if (readCommand(str, cmd)) {
                if (cmd == "color") {
                    if (getValues(str, values) < 3) {
                        *err << "Color with insufficent parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 3);
                        n->_vec3 = new ParametricColor();
                        for (int ci = 0; ci < 3; ci++)
                            n->_vec3->_color[ci] = values[ci];
                    }
                } else if (indices.find(cmd) != indices.end()) {
                    if (getValues(str, values) < 1) {
                        *err << cmd << " with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_coefficients[indices[cmd]] = values[0];
                    }
                } else {
                    *err << "Error: command " << cmd << " not recognized at "; errLine(str.tellg());
                }
                findCloseParen(str);
            }
        }
    } while (true);
}

void SceneLoader::SetSphereDefaults(SceneGroup *n)
{
    if (n->_sphere->_material == NULL)
    {
        n->_sphere->_material = new ParametricMaterial();
        SetMaterialDefaults(n->_sphere->_material);
    }
    if (n->_sphere->_radius == NULL)
    {
        n->_sphere->_radius = new ConstValue(1);
    }
}

// no obj includes in as5; they come back in as6
bool SceneLoader::doInclude(istream &str, string& name)
{
    if (!getName(str, "include", name))
        return false;

    SceneGroup *n = new SceneGroup();
    groups[name] = n;
    n->_name = name;

    string file = getQuoted(str);
    n->_mesh = new OBJTriangleMesh(file);
    
    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR) {
            if (n->_meshMaterial == NULL) {
                n->_meshMaterial = new ParametricMaterial();
                SetMaterialDefaults(n->_meshMaterial);
            }
            return false;
        } else if (state == CLOSED) {
            if (n->_meshMaterial == NULL) {
                n->_meshMaterial = new ParametricMaterial();
                SetMaterialDefaults(n->_meshMaterial);
            }
            return true;
        } else if (state == OPEN) {
            string cmd;
            if (readCommand(str, cmd)) {
                if (cmd == "material") {
                    string matName = getString(str);
                    if (matName.empty()) {
                        *err << "No material name after material command at "; errLine(str.tellg());
                    } else if (materials[matName] == NULL) {
                        *err << "Unknown material " << matName << " referenced at "; errLine(str.tellg());
                    } else {
                        n->_meshMaterial = materials[matName];
                    }
                } else {
                    *err << "Error: command " << cmd << " not recognized at "; errLine(str.tellg());
                }
                findCloseParen(str);
            }
        }
    } while (true);
}

bool SceneLoader::doSphere(istream &str, string &name)
{
    if (!getName(str, "sphere", name))
        return false;

    SceneGroup *n = new SceneGroup();
    groups[name] = n;
    n->_name = name;

    n->_sphere = new ParametricSphere();

    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR) {
            SetSphereDefaults(n);
            return false;
        } else if (state == CLOSED) {
            SetSphereDefaults(n);
            return true;
        } else if (state == OPEN)
        {
            string cmd;
            vector<ParametricValue*> values;
            if (readCommand(str, cmd)) {
                if (cmd == "radius") {
                    if (getValues(str, values) < 1) {
                        *err << "Type with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_sphere->_radius = values[0];
                    }
                } else if (cmd == "material") {
                    string matName = getString(str);
                    if (matName.empty()) {
                        *err << "No material name after material command at "; errLine(str.tellg());
                    } else if (materials[matName] == NULL) {
                        *err << "Unknown material " << matName << " referenced at "; errLine(str.tellg());
                    } else {
                        n->_sphere->_material = materials[matName];
                    }
                } else {
                    *err << "Error: command " << cmd << " not recognized at "; errLine(str.tellg());
                }
                findCloseParen(str);
            }
        }
    } while (true);
}

void SceneLoader::SetLightDefaults(SceneGroup *n) {
    // set default values for unset variables
    if (n->_light->_angularFalloff == NULL)
        n->_light->_angularFalloff = new ConstValue(0);
    if (n->_light->_color == NULL) {
        n->_light->_color = new ParametricColor();
        n->_light->_color->_color[0] = new ConstValue(0);
        n->_light->_color->_color[1] = new ConstValue(0);
        n->_light->_color->_color[2] = new ConstValue(0);
    }
    if (n->_light->_deadDistance == NULL)
        n->_light->_deadDistance = new ConstValue(.1);
    if (n->_light->_falloff == NULL)
        n->_light->_falloff = new ConstValue(0);
    if (n->_light->_type == NULL)
        n->_light->_type = new ConstValue(LIGHT_AMBIENT);
}

bool SceneLoader::doLight(istream &str, string &name)
{
    if (!getName(str, "light", name))
        return false;

    SceneGroup *n = new SceneGroup();
    groups[name] = n;
    n->_name = name;

    n->_light = new ParametricLight();

    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR) {
            SetLightDefaults(n);
            return false;
        } else if (state == CLOSED) {
            SetLightDefaults(n);
            return true;
        }
        else if (state == OPEN)
        {
            string cmd;
            vector<ParametricValue*> values;
            if (readCommand(str, cmd)) {
                if (cmd == "type") {
                    if (getValues(str, values) < 1) {
                        *err << "Type with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_light->_type = values[0];
                    }
                } else if (cmd == "falloff") {
                    if (getValues(str, values) < 1) {
                        *err << "Falloff with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_light->_falloff = values[0];
                    }
                } else if (cmd == "deaddistance") {
                    if (getValues(str, values) < 1) {
                        *err << "deaddistance with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_light->_deadDistance = values[0];
                    }
                } else if (cmd == "angularfalloff") {
                    if (getValues(str, values) < 1) {
                        *err << "angularfalloff with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_light->_angularFalloff = values[0];
                    }
                } else if (cmd == "color") {
                    if (getValues(str, values) < 3) {
                        *err << "color with insufficient parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 3);
                        n->_light->_color = new ParametricColor();
                        n->_light->_color->_color[0] = values[0];
                        n->_light->_color->_color[1] = values[1];
                        n->_light->_color->_color[2] = values[2];
                    }
                } else {
                    *err << "Error: command " << cmd << " not recognized at "; errLine(str.tellg());
                }
                findCloseParen(str);
            }
        }
    } while (true);
}

void SceneLoader::SetCameraDefaults(SceneGroup *n)
{
    if (n->_camera->_perspective == NULL)
        n->_camera->_perspective = new ConstValue(1); // default perspective to 1
    
    // set default values for the frustum
    if (n->_camera->_frustum[0] == NULL)
        n->_camera->_frustum[0] = new ConstValue(-.33);
    if (n->_camera->_frustum[1] == NULL)
        n->_camera->_frustum[1] = new ConstValue(+.33);
    if (n->_camera->_frustum[2] == NULL)
        n->_camera->_frustum[2] = new ConstValue(-.33);
    if (n->_camera->_frustum[3] == NULL)
        n->_camera->_frustum[3] = new ConstValue(+.33);
    if (n->_camera->_frustum[4] == NULL)
        n->_camera->_frustum[4] = new ConstValue(-1);
    if (n->_camera->_frustum[5] == NULL)
        n->_camera->_frustum[5] = new ConstValue(-100);
}

bool SceneLoader::doCamera(istream &str, string &name)
{
    if (!getName(str, "camera", name))
        return false;

    SceneGroup *n = new SceneGroup();
    groups[name] = n;
    n->_name = name;

    n->_camera = new ParametricCamera();
    

    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR) {
            SetCameraDefaults(n);
            return false;
        } else if (state == CLOSED) {
            SetCameraDefaults(n);
            return true;
        }
        else if (state == OPEN)
        {
            string cmd;
            vector<ParametricValue*> values;
            string sides = "lrbtnf";
            int side = 0;
            if (readCommand(str, cmd)) {
                if (cmd == "perspective") {
                    if (getValues(str, values) < 1) {
                        *err << "Perspective with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_camera->_perspective = values[0];
                    }
                }
                else if (cmd.size() == 1 && (side = (int)sides.find(cmd)) != string::npos) {
                    if (getValues(str, values) < 1) {
                        *err << "l with no parameters at "; errLine(str.tellg());
                    } else {
                        cleanAfter(values, 1);
                        n->_camera->_frustum[side] = values[0];
                    }
                }
                else
                {
                    *err << "Error: command " << cmd << " not recognized at "; errLine(str.tellg());
                }
                findCloseParen(str);
            }
        }
    } while (true);
}

bool SceneLoader::doG(istream &str, string &name)
{
    name = getString(str);
    if (name.empty()) {
        *err << "Couldn't read group name at "; errLine(str.tellg());
        return false;
    }

    if (groups[name] != NULL)
    {
        *err << "Illegal re-use of group name \"" << name << "\" at ";
        curPos(*err,str.tellg());
        *err << endl;
        return false;
    }

    SceneGroup *n = new SceneGroup();
    groups[name] = n;
    n->_name = name;

    do {
        int state = findOpenOrClosedParen(str);
        if (state == ERROR)
            return false;
        else if (state == CLOSED)
            return true;
        else if (state == OPEN)
        {
            string cmd;
            if (readCommand(str, cmd)) {
                if (cmd != "I")
                {
                    *err << "Command other than I from G at ";
                    curPos(*err,str.tellg());
                    *err << endl;
                }
                string iname;
                SceneInstance *newNode;
                if ((newNode = doI(str,iname)) != NULL) {
                    n->_children.push_back( newNode );
                }
                findCloseParen(str);
            }
        }
    } while (true);
}


bool SceneLoader::doRender(istream &str, string &name)
{
    name = getString(str);
    if (name.empty())
    {
        *err << "Trying to render group without specifying a name at ";
        curPos(*err,str.tellg());
        *err << endl;
        return false;
    }

    if (groups[name] == NULL)
    {
        *err << "Trying to render group not found \"" << name << "\" at ";
        curPos(*err,str.tellg());
        *err << endl;
        return false;
    }

    root->_child = groups[name];

    return true;
}

bool SceneLoader::buildScene(string filename)
{
    buildEndlineTable(filename);

    ifstream file(filename.c_str());
    string line;
    int lastPos = 0;
    while (findOpenParen(file))
    {
        file.tellg();
        if (readCommand(file, line)) {
            if (line == "Include")
            {
                string instName;
                if (doInclude(file, instName))
                {
                    cout << "included " << instName << endl;
                }
                else
                {
                    cout << "mangled include at ";
                    curPos(cout, file.tellg());
                    cout << endl;
                }
            }
            else if (line == "Sphere")
            {
                string gname;
                if (doSphere(file, gname))
                {
                    cout << "read sphere " << gname << endl;
                }
                else
                {
                    *err << "mangled sphere command at "; errLine(file.tellg());
                }
            }
            else if (line == "Material")
            {
                string gname;
                if (doMaterial(file, gname))
                {
                    cout << "read material " << gname << endl;
                }
                else
                {
                    *err << "mangled material command at "; errLine(file.tellg());
                }
            }
            else if (line == "Light")
            {
                string gname;
                if (doLight(file, gname))
                {
                    cout << "read light " << gname << endl;
                }
                else
                {
                    *err << "mangled light command at "; errLine(file.tellg());
                }
            }
            else if (line == "Camera")
            {
                string gname;
                if (doCamera(file, gname))
                {
                    cout << "read camera " << gname << endl;
                }
                else
                {
                    *err << "mangled camera command at "; errLine(file.tellg());
                }
            }
            else if (line == "I")
            {
                *err << "Error: Instance commands must belong to a group, but I found in global scope at "; errLine(file.tellg());
                /*string iname; // code to handle I at global scope (doesn't make much sense now that instance names skip the names table)
                if (doI(file, iname))
                {
                    cout << "got an instance named " << iname << endl;
                }*/
            }
            else if (line == "G")
            {
                string iname;
                if (doG(file, iname))
                {
                    cout << "got a group named " << iname << endl;
                }
            }
            else if (line == "Render")
            {
                string iname;
                if (doRender(file, iname))
                {
                    cout << "did render " << iname << endl;
                }
            }
            else
            {
                *err << "command not recognized: " << line << endl;
            }
            findCloseParen(file);
        } else {

        }

        lastPos = file.tellg();
    }

    return true;
}
