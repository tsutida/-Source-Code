using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using UnityEngine.UI;

public class PauseButton : MonoBehaviour {
    Button button;
    [SerializeField] private GameObject Pause;
    // Use this for initialization
    void Start () {
        button = GameObject.Find("Canvas/Pause/ResumeButton").GetComponent<Button>();
        //ボタンが選択された状態になる
        button.Select();
    }
	
	// Update is called once per frame
	void Update () {		
	}

    public void StartOnClick()
    {

        SceneManager.LoadScene("Start");
    }

    public void ResumeOnClick()
    {
        Pause.SetActive(false);
        Time.timeScale = 1f;
        bg_Active.audioSource.UnPause();
    }



}
