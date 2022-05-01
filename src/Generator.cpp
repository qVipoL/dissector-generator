#include "../include/Generator/Generator.h"

#include "../include/Generator/Struct/LocalElement.h"
#include "../include/Generator/Struct/SwitchElement.h"
#include "../include/util/util.h"

/* Private */

void Generator::process() {
    if (!_tree) throw runtime_error("Starting node not found.");

    this->processNode(_tree);
}

void Generator::processNode(AST *node) {
    if (!node) throw runtime_error("Node not found.");

    switch (node->getType()) {
        case AST_PROTO:
            this->processProto(node);
            break;
        case AST_PROTO_DECL:
            this->processProtoDecl(node);
            break;
        case AST_ENDIAN:
            this->processEndian(node);
            break;
        case AST_PROTO_DETAILS:
            this->processProtoDetails(node);
            break;
        case AST_DISSECTOR_ENTRY:
            this->processDissectorEntry(node);
            break;
        case AST_DISSECTOR_TABLE:
            this->processDissectorTable(node);
            break;
        case AST_ENUM:
            this->processEnum(node);
            break;
        case AST_ENUM_BODY_DECL:
            this->processEnumBodyDecl(node);
            break;
        case AST_ENUM_BODY_DEFAULT:
            this->processEnumBodyDefault(node);
            break;
        case AST_STRUCT:
            this->processStruct(node);
            break;

        default:
            throw runtime_error("AST Node type not supported.");
    }
}

void Generator::processProto(AST *node) {
    for (AST *child : node->getChildren())
        this->processNode(child);
}

void Generator::processProtoDecl(AST *node) {
    for (AST *child : node->getChildren())
        this->processNode(child);
}

void Generator::processEndian(AST *node) {
    string type = node->getId(0);
    EndianType endianType = type.compare("big") == 0 ? ENDIAN_BIG : ENDIAN_LITTLE;
    _endian_type = endianType;
}

void Generator::processProtoDetails(AST *node) {
    string name = node->getString(0), details = node->getString(1);
    _dissectors[name] = new Dissector(details);
}

void Generator::processDissectorEntry(AST *node) {
    string name = node->getId(0), structName = node->getId(1);
    Dissector *dissector = _dissectors[name];
    StructInfo *packetStruct = _structs[structName];
    ostringstream stringStream;

    if (!dissector) {
        stringStream << "[Line " << node->getLineN() << "] Referance to dissector: " << name << " was not found." << endl;
        throw runtime_error(stringStream.str());
    }

    if (!packetStruct) {
        stringStream << "[Line " << node->getLineN() << "] Referance to struct: " << structName << " was not found." << endl;
        throw runtime_error(stringStream.str());
    }

    dissector->setStructEntry(structName);
    packetStruct->setIsTopLevel(true);
    packetStruct->setDissector(dissector);
    _dissector_struct = packetStruct;
}

void Generator::processDissectorTable(AST *node) {
    string name = node->getId(0), table = node->getString(0), table_entry = node->getString(1);
    Dissector *dissector = _dissectors[name];
    ostringstream stringStream;

    if (!dissector) {
        stringStream << "[Line " << node->getLineN() << "] Referance to dissector: " << name << " was not found." << endl;
        throw runtime_error(stringStream.str());
    }

    dissector->setTable(table);
    dissector->setTableEntry(table_entry);
}

void Generator::processEnum(AST *node) {
    string name = node->getId(0), type = node->getId(1);
    int bytes = baseTypeLen(type);
    ostringstream stringStream;

    if (bytes < 0) {
        stringStream << "[Line " << node->getLineN() << "] Type: " << type << " no such type." << endl;
        throw runtime_error(stringStream.str());
    }

    _curr_enum = new EnumInfo(name, type, bytes);

    for (AST *child : node->getChildren())
        this->processNode(child);

    _enums[name] = _curr_enum;
    _curr_enum = NULL;
}

void Generator::processEnumBodyDecl(AST *node) {
    EnumElement *element = new EnumElement(node->getId(0), node->getNumber(0), node->getString(0));
    _curr_enum->addElement(element);
}

void Generator::processEnumBodyDefault(AST *node) {
    _curr_enum->setHasDefault(true);
    _curr_enum->setDefaultString(node->getString(0));
}

void Generator::processStruct(AST *node) {
    string name = node->getId(0);
    _curr_struct = new StructInfo(name, this);

    for (AST *child : node->getChildren())
        this->processStructChild(child);

    _structs[name] = _curr_struct;
    _curr_struct = NULL;
}

void Generator::processStructChild(AST *node) {
    if (node->getType() == AST_STRUCT_BODY_LOCAL) {
        _curr_struct->addElement(this->processLocalElement(node->getChildren()[0]));
    } else {
        _curr_struct->addElement(this->processSwitch(node->getChildren()[0]));
    }
}

StructElement *Generator::processLocalElement(AST *node) {
    string type = node->getId(0);
    string id = node->getId(1) != "" ? node->getId(1) : node->getString(0);
    StructElement *localElement = new LocalElement(TYPE_LOCAL_ELEMENT);

    if (node->getNumber(0).compare("") != 0)  // if bit mask
        localElement->setBitMask(decToHex(node->getNumber(0)));

    localElement->setId(id);
    localElement->setType(type);
    localElement->setGenerator(this);
    localElement->setParent(_curr_struct);

    return localElement;
}

StructElement *Generator::processSwitch(AST *node) {
    int idx = 0;
    FieldPath *field_path = this->processFieldPath(node->getChildren()[0]);
    StructElement *switchElement = new SwitchElement(TYPE_SWITCH);
    string label_text;

    switchElement->setConditionPath(field_path);
    switchElement->setGenerator(this);
    switchElement->setParent(_curr_struct);

    for (AST *child : node->getChildren()) {
        if (idx++ == 0) continue;

        SwitchCase *case_element = new SwitchCase(this);
        ostringstream stringStream;

        if (child->getType() == AST_CASE_DEFAULT) {
            case_element->setIsDefault(true);
            label_text = "default";
        } else if (child->getChildren()[0]->getId(0) != "") {  // ID
            case_element->setCaseValue(child->getChildren()[0]->getId(0));
            label_text = child->getChildren()[0]->getId(0);
        } else {  // NUMBER
            case_element->setCaseValue(child->getChildren()[0]->getNumber(0));
            label_text = child->getChildren()[0]->getNumber(0);
        }

        if (child->getType() == AST_CASE_DEFAULT)
            this->processCaseBody(case_element, child->getChildren()[0], label_text);
        else
            this->processCaseBody(case_element, child->getChildren()[1], label_text);

        stringStream << _curr_struct->getName() << "_" << idx;
        case_element->setLocalName(stringStream.str());
        switchElement->addCase(case_element);
    }

    return switchElement;
}

void Generator::processCaseBody(SwitchCase *case_element, AST *node, string label_text) {
    case_element->setTypeName(label_text);
    if (node->getId(0).compare("void") == 0) {
        case_element->setIsVoid(true);
    } else {
        node = node->getChildren()[0];
        StructElement *local_element = this->processLocalElement(node);
        case_element->addElement(local_element);
    }
}

FieldPath *Generator::processFieldPath(AST *node) {
    return new FieldPath(node->getChildren()[0]);
}

string Generator::generate() {
    if (!this->isMissingDeclarations()) {
        this->setupReferences();

        this->setupItemReferences();

        if (semantic_error) throw runtime_error("Semantic error...");

        return this->generateCode();
    }

    throw runtime_error("Error: Missing struct declarations.");
}

string Generator::generateCode() {
    ostringstream stringStream;
    map<string, Dissector *>::iterator di;
    map<string, EnumInfo *>::iterator ei;
    map<string, StructInfo *>::iterator si;

    // stringStream << endl;
    // stringStream << "=== LUA Dissector ===" << endl;
    // stringStream << endl;

    for (di = _dissectors.begin(); di != _dissectors.end(); di++)
        stringStream << this->generateDissector(di->first);

    for (ei = _enums.begin(); ei != _enums.end(); ei++)
        stringStream << this->generateEnum(ei->first);

    for (di = _dissectors.begin(); di != _dissectors.end(); di++) {
        stringStream << this->generateProtoFields(di->first);
        stringStream << this->generateProtoStructs(di->first);
        stringStream << this->generateProtoEnding(di->first);
    }

    return stringStream.str();
}

bool Generator::isMissingDeclarations() {
    vector<StructInfo *> struct_stack;
    bool missing = false;

    if (_dissector_struct == NULL || _dissector_struct->checkMissing(struct_stack))
        missing = true;

    return missing;
}

void Generator::setupReferences() {
    vector<FieldPath *> needed;

    _dissector_struct->setupReferences(needed);

    if (needed.size() > 0)
        semantic_error = true;
}

void Generator::setupItemReferences() {
    vector<FieldPath *> needed_items;

    _dissector_struct->setupItemReferences(needed_items);

    if (needed_items.size() > 0)
        semantic_error = true;
}

string Generator::generateDissector(string name) {
    ostringstream stringStream;

    stringStream << name << "_proto = Proto(";
    stringStream << "\"" << name << "\", ";
    stringStream << "\"" << _dissectors[name]->getDetails() << "\")" << endl;
    stringStream << endl;

    return stringStream.str();
}

string Generator::generateEnum(string name) {
    ostringstream stringStream;
    EnumInfo *enum_info = _enums[name];

    stringStream << "local " << name << " = {" << endl;

    for (EnumElement *element : enum_info->getElements()) {
        stringStream << " [" << element->getValue() << "] = ";
        stringStream << "\"" << element->getString() << "\"," << endl;
    }

    stringStream << "}" << endl
                 << endl;

    for (EnumElement *element : enum_info->getElements())
        stringStream << "local " << element->getId() << " = " << element->getValue() << endl;

    stringStream << endl;

    return stringStream.str();
}

string Generator::generateProtoFields(string name) {
    ostringstream stringStream;
    Dissector *dissector = _dissectors[name];
    vector<string> structs_left, field_names, expert_names;
    string struct_name;
    StructInfo *curr_struct;

    structs_left.push_back(dissector->getStructEntry());

    while (structs_left.size() > 0) {
        struct_name = structs_left[0];
        curr_struct = _structs[struct_name];
        structs_left.erase(next(structs_left.begin(), 0));

        stringStream << curr_struct->generateLuaFields(name, &structs_left, &field_names, &expert_names);
    }

    stringStream << endl;

    stringStream << name << "_proto.fields = {" << endl;
    for (string field : field_names)
        stringStream << "  " << field << "," << endl;
    stringStream << "}" << endl
                 << endl;

    return stringStream.str();
}

string Generator::generateProtoStructs(string name) {
    ostringstream stringStream;
    Dissector *dissector = _dissectors[name];
    vector<string> structs_left;
    StructInfo *curr_struct;

    structs_left.push_back(dissector->getStructEntry());

    while (structs_left.size() > 0) {
        curr_struct = _structs[structs_left[0]];
        structs_left.erase(next(structs_left.begin(), 0));

        stringStream << curr_struct->generateLuaStructDissect(name, &structs_left);
    }

    return stringStream.str();
}

string Generator::generateProtoEnding(string name) {
    Dissector *proto = _dissectors[name];
    ostringstream stringStream;

    stringStream << "ent_table = Dissector.get(\"" << proto->getTable() << "\")" << endl;
    stringStream << "ent_table:add("
                 << "\"" << proto->getTableEntry() << "\"";
    stringStream << ", " << name << "_proto)" << endl;

    return stringStream.str();
}

/* Public */

Generator::Generator(AST *tree) {
    _tree = tree;
    semantic_error = false;
}

Generator::~Generator() {
    map<string, Dissector *>::iterator di;
    map<string, EnumInfo *>::iterator ei;
    map<string, StructInfo *>::iterator si;

    if (_curr_struct != NULL)
        delete _curr_struct;
    if (_curr_enum != NULL)
        delete _curr_enum;

    for (di = _dissectors.begin(); di != _dissectors.end(); di++)
        delete di->second;

    for (ei = _enums.begin(); ei != _enums.end(); ei++)
        delete ei->second;

    for (si = _structs.begin(); si != _structs.end(); si++)
        delete si->second;
}

EndianType Generator::getEndianType() {
    return _endian_type;
}

StructInfo *Generator::getStruct(string struct_name) {
    return _structs[struct_name];
}

EnumInfo *Generator::getEnum(string enum_name) {
    return _enums[enum_name];
}

string Generator::generateLua() {
    this->process();
    return this->generate();
}