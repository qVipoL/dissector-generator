#include "../include/Generator/Generator.h"

/* Private */

void Generator::process() {
    if (!_tree) throw runtime_error("Starting node not found.");

    this->processNode(_tree);

    cout << "done" << endl;
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

    packetStruct->setIsTopLevel(true);
    packetStruct->setDissector(dissector);
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
    int bytes = TYPE_TO_BYTE[type];
    ostringstream stringStream;

    if (bytes == 0) {
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
    _curr_struct = new StructInfo(name);

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
    StructElement *localElement = new StructElement(TYPE_LOCAL_ELEMENT);

    localElement->setId(id);
    localElement->setType(type);

    return localElement;
}

StructElement *Generator::processSwitch(AST *node) {
    int idx = 0;
    FieldPath *field_path = this->processFieldPath(node->getChildren()[0]);
    StructElement *switchElement = new StructElement(TYPE_SWITCH);
    string label_text;
    switchElement->setConditionPath(field_path);

    for (AST *child : node->getChildren()) {
        if (idx++ == 0) continue;

        SwitchCase *case_element = new SwitchCase();
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
    node = node->getChildren()[0];
    case_element->setTypeName(label_text);
    if (node->getType() != AST_LOCAL_ELEMENT) {
        case_element->setIsVoid(true);
    } else {
        StructElement *local_element = this->processLocalElement(node);
        case_element->addElement(local_element);
    }
}

FieldPath *Generator::processFieldPath(AST *node) {
    return new FieldPath(node->getChildren()[0]);
}

/* Public */

Generator::Generator(AST *tree) {
    _tree = tree;
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

string Generator::generateLua() {
    this->process();
    return "";
}