using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Target : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        Invoke("Road", 3.0f);
 
    }
    void Road()
    {
        SceneManager.LoadScene("Stage1");

    }
}