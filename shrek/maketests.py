import os

charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12567890!@#$%^&*()`~-_=+[{]}\|;:'\",<.>/?"

for char in charset:
    os.system("mkdir " + char)

for dirn in charset:
    for char in charset:
        os.system("touch " + dirn + "/" + char)
