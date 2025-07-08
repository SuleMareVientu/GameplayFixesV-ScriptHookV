import xml.etree.ElementTree as ET
import os

def parse_xml_and_print(xml_content):
    """
    Parses the given XML content and prints formatted strings
    based on the 'rage__NMBehavior' and 'rage__NMParam' elements.

    Args:
        xml_content (str): A string containing the XML data.
    """
    try:
        root = ET.fromstring(xml_content)

        # Find all 'Item' elements that represent 'rage__NMBehavior'
        # We look for 'behaviors/Item' where the 'type' attribute is 'rage__NMBehavior'
        behaviors = root.findall('./behaviors/Item[@type="rage__NMBehavior"]')

        if not behaviors:
            print("No 'rage__NMBehavior' items found in the XML.")
            return

        # Iterate through each found behavior
        for behavior in behaviors:
            behavior_name_element = behavior.find('name')
            behavior_name = behavior_name_element.text if behavior_name_element is not None else "UNKNOWN_BEHAVIOR"

            # Find the 'params' section within the current behavior
            params_section = behavior.find('params')

            if params_section is None:
                print(f"Warning: No 'params' section found for behavior '{behavior_name}'.")
                continue

            # Find all 'Item' elements within the 'params' section
            # that represent 'rage__NMParam'
            params = params_section.findall('Item[@type="rage__NMParam"]')

            if not params:
                print(f"Warning: No 'rage__NMParam' items found for behavior '{behavior_name}'.")
                continue

            with open('NMMessage.cpp', 'a', encoding='utf-8') as f:
                f.write(f"struct NMMessage{behavior_name[0].upper() + behavior_name[1:]}\n")
                f.write("{\n")

                func = []

                # Iterate through each parameter within the current behavior
                for param in params:
                    param_name_element = param.find('name')
                    param_init_element = param.find('init')
                    param_type_element = param.find('type') # Get the type element

                    param_name = param_name_element.text if param_name_element is not None else "UNKNOWN_PARAM"
                    param_init = param_init_element.text if param_init_element is not None else "UNKNOWN_INIT"
                    param_type = param_type_element.text if param_type_element is not None else "UNKNOWN_TYPE"

                    # Format param_init based on its type
                    formatted_param_init = param_init
                    if param_type == 'int':
                        f.write(f"    int {param_name} = {param_init};\n")
                    elif param_type == 'bool':
                        f.write(f"    bool {param_name} = {param_init};\n")
                    elif param_type in ['float']:
                        formatted_param_init = f'{param_init}f'
                        f.write(f"    float {param_name} = {formatted_param_init};\n")
                    elif param_type == 'string':
                        formatted_param_init = f'"{param_init}"'
                        f.write(f"    std::string {param_name} = {formatted_param_init};\n")
                    elif param_type in ['vector3']:
                        formatted_param_init = param_init.replace(",", "f,")
                        formatted_param_init = f'{formatted_param_init}f'
                        f.write(f"    Vector3 {param_name}{"{"}{formatted_param_init}{"}"};\n")

                    # Print the formatted string
                    if param_type == 'string':
                        func.append(f"        nGame::SetNMMessageParam(msgPtr, \"{param_name}\", {param_name}.c_str());\n")
                    elif param_type in ['vector3']:
                        func.append(f"        nGame::SetNMMessageParam(msgPtr, \"{param_name}\", {param_name}.x, {param_name}.y, {param_name}.z);\n")
                    else:
                        func.append(f"        nGame::SetNMMessageParam(msgPtr, \"{param_name}\", {param_name});\n")

                f.write("    void operator()(NmMessagePtr msgPtr)\n")
                f.write("    {\n")
                f.write(f"{''.join(func)}")
                f.write("        return;\n")
                f.write("    }\n")
                f.write("};\n")
                f.write("\n")

    except ET.ParseError as e:
        print(f"Error parsing XML: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")





if os.path.exists("NMMessage.cpp"):
    os.remove("NMMessage.cpp")

with open('behaviours.xml', 'r', encoding='utf-8') as f:
    parse_xml_and_print(f.read())

with open('NMMessage.cpp', 'r') as file:
    file_contents = file.read()

updated_contents = file_contents.replace(".000f", ".0f")
updated_contents = updated_contents.replace(".100f", ".1f")
updated_contents = updated_contents.replace(".200f", ".2f")
updated_contents = updated_contents.replace(".300f", ".3f")
updated_contents = updated_contents.replace(".400f", ".4f")
updated_contents = updated_contents.replace(".500f", ".5f")
updated_contents = updated_contents.replace(".600f", ".6f")
updated_contents = updated_contents.replace(".700f", ".7f")
updated_contents = updated_contents.replace(".800f", ".8f")
updated_contents = updated_contents.replace(".900f", ".9f")
updated_contents = updated_contents.replace(".00f", ".0f")
updated_contents = updated_contents.replace(".10f", ".1f")
updated_contents = updated_contents.replace(".20f", ".2f")
updated_contents = updated_contents.replace(".30f", ".3f")
updated_contents = updated_contents.replace(".40f", ".4f")
updated_contents = updated_contents.replace(".50f", ".5f")
updated_contents = updated_contents.replace(".60f", ".6f")
updated_contents = updated_contents.replace(".70f", ".7f")
updated_contents = updated_contents.replace(".80f", ".8f")
updated_contents = updated_contents.replace(".90f", ".9f")
updated_contents = updated_contents.replace(" 0f;", " 0.0f;")
updated_contents = updated_contents.replace(" 2f;", " 2.0f;")
updated_contents = updated_contents.replace(" 600f;", " 600.0f;")
updated_contents = updated_contents.replace("0f, 0f, 0f", "0.0f, 0.0f, 0.0f")
updated_contents = updated_contents.replace("0f, 0f, 1f", "0.0f, 0.0f, 1.0f")
updated_contents = updated_contents.replace("1f, 0f, 0f", "1.0f, 0.0f, 0.0f")
updated_contents = updated_contents.replace("0f, 0f, -1f", "0.0f, 0.0f, -1.0f")
updated_contents = updated_contents.replace("    int animSource = ART::kITSourceCurrent;", "    int animSource = 0; // ART::kITSourceCurrent")

with open('NMMessage.cpp', 'w') as file:
    file.write(updated_contents)