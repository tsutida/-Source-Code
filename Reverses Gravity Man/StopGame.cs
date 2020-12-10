using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StopGame : MonoBehaviour {
    [SerializeField]private GameObject Pause;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKey(KeyCode.Escape))
        {
            Pause.SetActive(true);
            Time.timeScale = 0f;
            bg_Active.audioSource.Pause();
        }
        
    }
}
