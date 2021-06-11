import xml.etree.ElementTree as et

file_name = "v1.circ"

xmlTree = et.parse(file_name)

root = xmlTree.getroot()

for element in root.findall("circuit"):
    for i in element.findall("comp"):
        if i.get("name") == "ROM":
            for j in i.findall("a"):
                if j.get("name") == "contents":
                    j.text = "addr/data: 8 20 \naaaaa bbbbb ccccc eeeee fffff 248*0 fffda fffee fff"

xmlTree.write(file_name, encoding='UTF-8', xml_declaration=True)
