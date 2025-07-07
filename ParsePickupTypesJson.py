# Requires Python3
import os
import json
import urllib.request

def joaat(s: str) -> int:
    """
    Calculates the JOAAT (Jenkins One-At-A-Time) hash of a string.

    Args:
        s: The input string.

    Returns:
        The JOAAT hash as an unsigned 32-bit integer.
    """
    if s is None or not s:
        return 0

    hash_val = 0
    for char_code in s.encode('utf-8'):  # Iterate over byte values
        b = char_code
        if ord('A') <= b <= ord('Z'):
            b = b | (1 << 5)  # Convert to lowercase
        elif b == ord('\\'):
            b = ord('/')

        hash_val = (hash_val + b) & 0xFFFFFFFF  # Ensure 32-bit unsigned
        hash_val = (hash_val + (hash_val << 10)) & 0xFFFFFFFF
        hash_val = (hash_val ^ (hash_val >> 6)) & 0xFFFFFFFF

    # Finalize hash
    hash_val = (hash_val + (hash_val << 3)) & 0xFFFFFFFF
    hash_val = (hash_val ^ (hash_val >> 11)) & 0xFFFFFFFF
    hash_val = (hash_val + (hash_val << 15)) & 0xFFFFFFFF
    
    return hash_val

try:
    if (os.path.isfile("pickupTypes.json")):
        with open("pickupTypes.json", 'r', encoding="UTF-8") as f:
            data = json.load(f)
    else:
        data = json.load(urllib.request.urlopen("https://github.com/DurtyFree/gta-v-data-dumps/raw/refs/heads/master/pickupTypes.json"))

    if os.path.exists("wpPickupMap.txt"):
        os.remove("wpPickupMap.txt")

    with open('wpPickupMap.txt', 'a', encoding='utf-8') as f:
        f.write("static const std::unordered_map<unsigned int, unsigned int> wpPickupMap = {\n")
        for item in data: # item is expected to be a dictionary
            if isinstance(item, dict):
                pickup = ""
                model = ""
                for key, value in item.items():
                    if key == "Name":
                        pickup = value
                    if key == "Model":
                        model = value
                        if "VEHICLE" not in pickup.upper() and "AMMO" not in pickup.upper() and "WEAPON" in pickup.upper():  
                            s = '	' + "{" + "Joaat(" + '"' + model + '"' + ')' + ", " + "Joaat(" + '"' + pickup + '"' + ')' + "},"
                            f.write(s + '\n')
                        
    with open('wpPickupMap.txt', 'r', encoding='utf-8') as f:
        lines = f.readlines()
        
    if lines:
        last_line_content = lines[-1].rstrip('\n')
        if last_line_content.endswith(','):
            modified_last_line = last_line_content[:-len(',')]
            lines[-1] = modified_last_line + '\n'
            with open('wpPickupMap.txt', 'w', encoding='utf-8') as f:
                f.writelines(lines)
                
    with open('wpPickupMap.txt', 'a', encoding='utf-8') as f:
        f.write("};\n")

except FileNotFoundError:
    print("Error: weapons.json not found. Please ensure the file exists in the same directory as the script.")
except json.JSONDecodeError:
    print("Error: Could not decode JSON from weapons.json. Check for malformed JSON syntax.")
except Exception as e:
    print("An unexpected error occurred: {}".format(e))