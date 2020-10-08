using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;

public class BinaryExporter
{
    [MenuItem("Tools/Export all BIN #_y")]
    private static void DoExportBinary()
    {
        string target_path = "..\\Bin\\Unity Level Export\\";
        if (!System.IO.Directory.Exists(target_path))
        {
            System.IO.Directory.CreateDirectory(target_path);
        }

        string scene_name = SceneManager.GetActiveScene().name;

        // Save paths
        Dictionary<string, string> filter = new Dictionary<string, string>();
      
        GameObject[] allModels = UnityEngine.Object.FindObjectsOfType<GameObject>();
        foreach (GameObject go in allModels)
        {
            MeshFilter mesh = go.GetComponent<MeshFilter>();
            if (mesh != null)
            {
                    
                string path = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(go));
                filter[path] = path;
                //
            }
        }
 

        using (System.IO.StreamWriter file = new System.IO.StreamWriter(target_path + scene_name + "_bin_modelPaths.txt"))
        {
            foreach (KeyValuePair<string, string> entry in filter)
            {
                file.WriteLine(entry.Key);
                // do something with entry.Value or entry.Key
            }
        }


        BinaryWriter bw;
        bw = new BinaryWriter(new FileStream(target_path + scene_name + "_bin.bin", FileMode.Create));

        GameObject[] allObjects = UnityEngine.Object.FindObjectsOfType<GameObject>();

        int totalAssetsSizeToExport = 0;
        foreach (GameObject go in allObjects)
        {
            MeshFilter mesh = go.GetComponent<MeshFilter>();
            if (mesh != null)
            {
                totalAssetsSizeToExport++;
            }
        }


        bw.Write(totalAssetsSizeToExport);
        foreach (GameObject go in allObjects)
        {
            MeshFilter mesh = go.GetComponent<MeshFilter>();
            if(mesh != null)
            {
                int index = Getindex(go, filter);
                bw.Write(go.transform.rotation.eulerAngles.x);
                bw.Write(go.transform.rotation.eulerAngles.y);
                bw.Write(go.transform.rotation.eulerAngles.z);
                bw.Write(go.transform.position.x);
                bw.Write(go.transform.position.y);
                bw.Write(go.transform.position.z);

                bw.Write(go.transform.localScale.x);
                bw.Write(go.transform.localScale.y);
                bw.Write(go.transform.localScale.z);
                bw.Write(index);
            }


        }

        bw.Close();

    }

    private static int Getindex(GameObject go, Dictionary<string, string> aDic)
    {
        int index = -1;
        foreach (KeyValuePair<string, string> entry in aDic)
        {
            index++;

            string path = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromSource(go));

            if (entry.Value == path)
            {
                break;
            }

            // do something with entry.Value or entry.Key
        }
        return index;
    }

}