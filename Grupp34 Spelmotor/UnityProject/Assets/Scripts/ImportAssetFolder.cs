using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;
using System.IO;

public class ImportAssetFolder : MonoBehaviour
{
    // Get everything in the Unity Assets folder
    // Replace everything but the .meta files

    [MenuItem("Tools/Import Asset Folder #k")]
    static void CopyAssets()
    {
        string targetPath   = "..\\IronWrought\\Bin\\Assets\\3D\\";// Engine Asset path, assumes Unity Project is in the same folder as Source,Bin and Lib.
        string unityPath    = "Assets\\3D\\";

        FileUtil.DeleteFileOrDirectory(unityPath);// Delete it so we can copy in the files
        FileUtil.CopyFileOrDirectory(targetPath, unityPath);
        
        AssetDatabase.Refresh();
    }
}


// Test: check for every file and replace everyhting but .meta files. Is incomplete!
/*
 * // can replace with FileUtil.ReplaceFile(src, dst), if need to find specific file or folder.
        // Gets all directories
        var unityInfo = new DirectoryInfo(unityPath);
        var unityDirectoryInfo = unityInfo.GetDirectories();
       
        string assetsStr = "Assets";
        List<string> unityDirectory = new List<string>();
        foreach (var file in unityDirectoryInfo)
        {
            var contents = new DirectoryInfo(file.ToString());
            var contentsFiles = contents.GetFiles();
            foreach (var subfile in contentsFiles)
            {
                int assetStrIndex = subfile.ToString().IndexOf(assetsStr);
                if (assetStrIndex >= 0)
                {
                    string trimmed = subfile.ToString().Remove(0, assetStrIndex);
                    unityDirectory.Add(trimmed);
                    print(unityDirectory[unityDirectory.Count - 1]);
                }
            }
        }

        var targetInfo = new DirectoryInfo(targetPath);
        var directoryInfo = targetInfo.GetDirectories();
        foreach (var file in directoryInfo)
        {
            int assetStrIndex = file.ToString().IndexOf(assetsStr);
            if (unityDirectoryInfo.Contains<System.IO.DirectoryInfo>(file))
            {
               // print("both contain : ");// nah
            }
            //print(file);
            // Get contents of directory
            var contents = new DirectoryInfo(file.ToString());
            var contentsFiles = contents.GetFiles();
            foreach (var cfile in contentsFiles)
            {
                //print(cfile);
            }
        }

        // Gets all loose .fbx
        targetInfo = new DirectoryInfo(targetPath);
        var fileInfo = targetInfo.GetFiles();
        foreach (var file in fileInfo) {
            //print(file);
        }
        */
