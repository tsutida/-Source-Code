using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Management : MonoBehaviour {
    void Start () {
    }
	
	// Update is called once per frame
	void Update () {
	}
    void GameOver()
    {
        SceneManager.LoadScene("ResultScene");
    }
     
    void GameClear()
    {
        SceneManager.LoadScene("ResultScene");
    }






}
