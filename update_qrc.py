import os
import xml.etree.ElementTree as ET
from xml.dom import minidom

def generate_qrc_file(light_dir, dark_dir, fonts_dir, output_qrc):
    # Create the root element for the .qrc file
    root = ET.Element("RCC")
    qresource = ET.SubElement(root, "qresource", {"prefix": "/"})

    # Add files from the light theme directory
    for root_dir, _, files in os.walk(light_dir):
        for file in files:
            file_path = os.path.relpath(os.path.join(root_dir, file), start=os.getcwd())
            ET.SubElement(qresource, "file").text = file_path

    # Add files from the dark theme directory
    for root_dir, _, files in os.walk(dark_dir):
        for file in files:
            file_path = os.path.relpath(os.path.join(root_dir, file), start=os.getcwd())
            ET.SubElement(qresource, "file").text = file_path

    # Add files from the fonts directory
    for root_dir, _, files in os.walk(fonts_dir):
        for file in files:
            file_path = os.path.relpath(os.path.join(root_dir, file), start=os.getcwd())
            ET.SubElement(qresource, "file").text = file_path

    # Prettify the XML
    rough_string = ET.tostring(root, encoding="utf-8", method="xml")
    parsed = minidom.parseString(rough_string)
    pretty_xml_as_string = parsed.toprettyxml(indent="  ")

    # Write the prettified XML to the .qrc file
    with open(output_qrc, "w", encoding="utf-8") as f:
        f.write(pretty_xml_as_string)

# Specify your directories and output file
light_theme_dir = "./materials/light_theme"
dark_theme_dir = "./materials/dark_theme"
fonts_dir = "./materials/fonts"  # Add your fonts directory here
output_qrc_file = "resources.qrc"

# Generate the .qrc file
generate_qrc_file(light_theme_dir, dark_theme_dir, fonts_dir, output_qrc_file)
