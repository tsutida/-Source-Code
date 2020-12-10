using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameClear : MonoBehaviour {
    [SerializeField] private GameObject obj;
    [SerializeField] private  bool Clearflag = false;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if(Clearflag == true)
        {
            this.gameObject.SetActive(false);

        }
	}
}
