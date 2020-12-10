using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Goal : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
    private void OnTriggerEnter2D(Collider2D collision)
    {
        //シーン内のMainTextを探し、GameClear命令を呼び出す
        if (collision.gameObject.tag == "Player")
        {
            GameObject.Find("Player").SendMessage("Goal");
        }
    }
}
