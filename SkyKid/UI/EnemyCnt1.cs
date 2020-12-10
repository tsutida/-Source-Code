using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class EnemyCnt1 : MonoBehaviour {
    [SerializeField] private Text SkyscText;
    [SerializeField] private Text GroundscText;
    [SerializeField] private Text BasescText;
	// Use this for initialization
	void Start () {		
	}
	
	// Update is called once per frame
	void Update () {
        SkyscText.text = Enemy1.SkyCnt.ToString();
        GroundscText.text = Enemy2.GroundCnt.ToString();
        BasescText.text = enemy_baseleft.BaseCnt.ToString();
    }
}
