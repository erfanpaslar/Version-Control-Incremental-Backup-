from tkinter import *
from tkinter.ttk import *
import os

window=Tk()

window.title("Version Control")
window.geometry('400x400')
window.resizable(width=False, height=False)
width, height = 400, 400
canvas = Canvas(window, width=width, height=height)
canvas.pack()


selected = IntVar()
checked  = IntVar()

def on_entry_click(event):
    
    if enteryVersion.get() == 'for example: 1, 2 ...':
       enteryVersion.delete(0, "end") 
       enteryVersion.insert(0, '') 

def log():
    directoryPath = enteryDirectoryPath.get()
    text = "\""+directoryPath+"\\log\\log.txt\""
    os.system(text)
    
def clicked():
    labelLog.config(text = "")
    global text
    fileName = enteryFileName.get()
    directoryPath = enteryDirectoryPath.get()
    version = enteryVersion.get()
    
    if directoryPath=="":
        directoryPath="."
    if fileName=="":
        labelLog.config(text = 'File name not valid')
        print("File name not valid")
        return
        
    if (selected.get()==0):#FUll Backup   ->  "main.exe <input.txt >output.txt"
        labelLog.config(text = "Don't make full backup again")
        text="mkdir \""+directoryPath+"\\temp\""
        
        os.system(text)

        text=directoryPath+"\\temp\\input.txt"
        print(text)
        with open(text,"w") as input:
            text="0\n"+fileName+"\n"+directoryPath+"\n"
            input.write(text)

        ####text="gcc -o main main.c incremental.c restore.c fullBackup.c"
        #if you dont have gcc compiler just compile the 3 .c files together and put it in this dir with name of main.c4
        #and if you dont. just dont delete the main.exe 
        ####os.system(text)                                          
        
        text="main.exe <\""+directoryPath+"\\temp\\input.txt\" >\""+directoryPath+"\\temp\\output.txt\""
        os.system(text)

    elif selected.get()==1:#Inc Backup
        text="mkdir \""+directoryPath+"\\temp\""
        os.system(text)

        text=directoryPath+"\\temp\\input.txt"
        print(text)
        with open(text,"w") as input:
            text="1\n"+fileName+"\n"+directoryPath+"\n"
            input.write(text)

        ####text="gcc -o main main.c incremental.c restore.c fullBackup.c"
        #if you dont have gcc compiler just compile the 3 .c files together and put it in this dir with name of main.c4
        #and if you dont. just dont delete the main.exe 
        ####os.system(text) 
        text="main.exe <\""+directoryPath+"\\temp\\input.txt\" >\""+directoryPath+"\\temp\\output.txt\""
        os.system(text)

    elif selected.get()==2:#restore
        try:
            version=int(version)
            text="mkdir \""+directoryPath+"\\temp\""
            os.system(text)

            text=directoryPath+"\\temp\\input.txt"
            with open(text,"w") as input:
                text="2\n"+fileName+"\n"+directoryPath+"\n"+str(version)+"\n"
                input.write(text)
            
            ####text="gcc -o main main.c incremental.c restore.c fullBackup.c"
            #if you dont have gcc compiler just compile the 3 .c files together and put it in this dir with name of main.c4
            #you can just compile main2.txt and put it in this directory as main.c TOO.
            #and if you dont. just dont delete the main.exe 
            ####os.system(text) 
            text="main.exe <\""+directoryPath+"\\temp\\input.txt\" >\""+directoryPath+"\\temp\\output.txt\""
            os.system(text)
        except ValueError:
            print("Version Not Valid")
            labelLog.config(text = 'Version Not Valid')

    if checked.get()==1:
        text = "del \""+directoryPath+"\\temp\\*\""
        os.system(text)
        labelLog.config(text = 'Temp deleted!')
    
       
checkBoxTempDel = Checkbutton(window, text='Delete temporary files ( needs a confomation! )', variable=checked)

radioBtnFull = Radiobutton(window,text='Full Backup  "just do it for the first time" :)', value=0, variable=selected)
radioBtnInc  = Radiobutton(window,text='Incremental Backup', value=1, variable=selected)
radioBtnRe   = Radiobutton(window,text='Restore', value=2, variable=selected)

labelLog            = Label(window, text="", foreground="#fa4659")
labelcr             = Label(window, text="Copyright © 2020 All rights reserved.", foreground="gray")

labelTop            = Label(window, text="Version Control")

enteryFileName      = Entry(window,width=30)

labelFileNmae       = Label(window, text="File Name: ")

enteryDirectoryPath = Entry(window,width=30)
labelDirectoryPath  = Label(window, text="Directory Path: ")

enteryVersion       = Entry(window,width=30)
labelVersion        = Label(window, text="Version Number: ")
enteryVersion.insert(0, 'for example: 1, 2 ...')
enteryVersion.bind('<FocusIn>', on_entry_click)

btnBackup = Button(window, text="\" DO \"", command=clicked)
btnLog = Button(window, text="Show Log", command=log)


labelcr.config(font=("", 7))
labelTop.config(font=("", 26))

canvas.create_window(width/2, 1*height/8, window=labelTop)

canvas.create_window(.8*width/4, 3*height/10, window=labelFileNmae)
canvas.create_window(2.3*width/4, 3*height/10, window=enteryFileName)

canvas.create_window(.8*width/4, 3.8*height/10, window=labelDirectoryPath)
canvas.create_window(2.3*width/4, 3.8*height/10, window=enteryDirectoryPath)

canvas.create_window(.8*width/4, 4.6*height/10, window=labelVersion)
canvas.create_window(2.3*width/4, 4.6*height/10, window=enteryVersion)


canvas.create_window(2*width/4, 5.7*height/10, window=radioBtnFull)
canvas.create_window(1.4*width/4, 6.6*height/10, window=radioBtnInc)
canvas.create_window(2.6*width/4, 6.6*height/10, window=radioBtnRe)

canvas.create_window(2*width/4, 7.5*height/10, window=checkBoxTempDel)

canvas.create_window(1.5*width/4, 8.4*height/10, window=btnBackup)
canvas.create_window(2.5*width/4, 8.4*height/10, window=btnLog)

canvas.create_window(2*width/4, 9.1*height/10, window=labelLog)
canvas.create_window(2*width/4, 9.65*height/10, window=labelcr)




window.mainloop()
