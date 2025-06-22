# Requires Python3
import os
import json
import urllib.request

try:
    if (os.path.isfile("weapons.json")):
        with open("weapons.json", 'r', encoding="UTF-8") as f:
            data = json.load(f)
    else:
        data = json.load(urllib.request.urlopen("https://github.com/DurtyFree/gta-v-data-dumps/raw/refs/heads/master/weapons.json"))

    listkeys = ["Name", "Category", "AmmoType", "DamageType", "Components"]
    listkeystints = ["Index"]
    listkeyscomponents = ["Name"]
    listkeysliveries = ["Name"]
    
    updated_data = [] 

    for item in data: # item is expected to be a dictionary
        if isinstance(item, dict):
            
            new_item_data = {}
            for key, value in item.items():
                if key == "Tints":
                    updated_tits = []
                    for subitem in item["Tints"]:
                        if isinstance(subitem, dict):
                            new_tits_item = {}
                            for subkey, subvalue in subitem.items():
                                if subkey in listkeystints:
                                    new_tits_item[subkey] = subvalue
                        updated_tits.append(new_tits_item)
                    new_item_data[key] = updated_tits
                    
                elif key == "Components":
                    updated_components = []
                    for subitem in item["Components"]:
                        if isinstance(subitem, dict):
                            new_components_item = {}
                            for subkey, subvalue in subitem.items():
                                if subkey in listkeyscomponents:
                                    new_components_item[subkey] = subvalue
                        updated_components.append(new_components_item)
                    new_item_data[key] = updated_components
                    
                elif key == "Liveries":
                    updated_liveries = []
                    for subitem in item["Liveries"]:
                        if isinstance(subitem, dict):
                            new_liveries_item = {}
                            for subkey, subvalue in subitem.items():
                                if subkey in listkeysliveries:
                                    new_liveries_item[subkey] = subvalue
                        updated_liveries.append(new_liveries_item)
                    new_item_data[key] = updated_liveries
                elif key in listkeys:
                    new_item_data[key] = value
                    
            updated_data.append(new_item_data)

        else:
            print(f"Warning: Skipping non-dictionary item: {item}")

    with open("weapons.json", 'w', encoding="UTF-8") as f:
        json.dump(updated_data, f, indent=4)
    print("JSON file updated successfully.")

except FileNotFoundError:
    print("Error: weapons.json not found. Please ensure the file exists in the same directory as the script.")
except json.JSONDecodeError:
    print("Error: Could not decode JSON from weapons.json. Check for malformed JSON syntax.")
except Exception as e:
    print("An unexpected error occurred: {}".format(e))