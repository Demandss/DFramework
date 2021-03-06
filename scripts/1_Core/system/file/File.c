/*
 This code powered by ©Demands.
 at 4/22/2022
 */

class DFile extends FileSystem
{
    protected FileAttr attributes;
    
    void DFile(string path = "")
    {
        if (path == string.Empty) return;

        string dir = GetDirectory(path);

        if (!FileExist(dir)) MakeDirectory(dir);

        string fileName;
        FileAttr fileAttr;

        FindFileHandle handler = FindFile(path,fileName,fileAttr, FindFileFlags.ALL);
        
        if (!handler) 
        {
            CreateFile(path);
        }

        SetFileName(fileName);
        SetAttributes(fileAttr);
        SetDirectory(dir);
        
        CloseFindFile(handler);
    };

    bool IsDirectory() { return (attributes & FileAttr.DIRECTORY) != 0; };

    bool IsHidden() { return (attributes & FileAttr.HIDDEN) != 0;};

    bool IsReadOnly() { return (attributes & FileAttr.READONLY) != 0;};

    bool IsValid() { return (attributes & FileAttr.INVALID) != 0;};

    void SetAttributes(FileAttr attr) { attributes = attr;};

    FileHandle OpenFile(FileMode mode)
    {
        return OpenFile(GetFullDirectory(), mode);
    };

    void Print(void var)
    {
        FileHandle file = OpenFile(FileMode.WRITE);
	    if (file == 0)
	    {
		    Logger.Warning("Failed to write data to the file /%1/ it may be corrupted!",GetFullDirectory());
            return;
        }

        FPrint(file, var);
		CloseFile(file);
    };

    void Println(void var)
    {
        FileHandle file = OpenFile(FileMode.WRITE);
	    if (file == 0)
	    {
		    Logger.Warning("Failed to write data to the file /%1/ it may be corrupted!",GetFullDirectory());
            return;
        }

        FPrintln(file, var);
		CloseFile(file);
    };

    int Read(void param_array, int length)
    {
        FileHandle file = OpenFile(FileMode.READ);
	    if (file == 0)
	    {
		    Logger.Warning("Failed to read data from the file /%1/ it may be corrupted!",GetFullDirectory());
            return -1;
        }

        return ReadFile(file,param_array,length);
    };

    bool Rename(string name)
    {
        if (!IsValid()) return false;

        if (FileExist(directory + name) || !CopyFile(GetFullDirectory(), directory + name))
        {
            Logger.Warning("File already exists at /%1/",GetDirectory());
            return false;
        }
    
        if (!DeleteFile(GetFullDirectory()))
            Logger.Warning("Failed to delete original file /%1/ to rename!",GetFullDirectory());

        SetFilename(name);

        return true;
    };

    bool MoveTo(string path)
    {
        if (!IsValid()) return false;

        if (FileExist(path + filename) || !CopyFile(GetFullDirectory(), path + filename))
        {
            Logger.Warning("File already exists at /%1/",path);
            return false;
        }

        if (!DeleteFile(GetFullDirectory()))
            Logger.Warning("Failed to delete original file /%1/ to move!",GetFullDirectory());

        SetFileName(fileName);
        SetDirectory(path);

        return true;
    };

    bool Copy(string path)
    {
        if (!IsValid()) return false;

        if (FileExist(path + filename) || !CopyFile(GetFullDirectory(), path + filename))
        {
            Logger.Warning("File already exists at /%1/",path);
            return false;
        }

        SetDirectory(path);

        return true;
    };

    override string GetDebugName()
    {
        return string.Format("{path=%1;filename=%2;directory=%3;hidden=%4;readonly=%5;valid=%6}",GetFullDirectory(),GetFileName(),GetDirectory(),IsHidden(),IsReadOnly(),IsValid());
    };

    override static bool GetFiles(string path, out array<ref DFile> files, FindFileFlags flag = FindFileFlags.ALL)
	{
        Logger.Warning("You can't get a list of files from %1!",this);
        return false;
    }
};