using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BONUS : MonoBehaviour {
    [SerializeField] private Text SkyBtext;//空の敵ボーナススコアテキスト
    [SerializeField] private Text GroundBtext;
    [SerializeField] private Text BaseBtext;
    private int Bscore1;
    private int Bscore2;
    private int Bscore3;
    private int point = 1000;
    private int point2 = 1000;
    private int point3 = 10000;
    // Use this for initialization
    void Start () {
        
    }

    // Update is called once per frame
    void Update () {
        SkyBtext.text = Bscore1.ToString();
        GroundBtext.text = Bscore2.ToString();
        BaseBtext.text = Bscore3.ToString();
    }

    void Ebounus()
    {
        
        if (Enemy1.SkyCnt >= 10)
        {
         
            Bscore1 = 1000;
            FindObjectOfType<Highscore>().AddPoint(point);
        }

        if (Enemy2.GroundCnt >= 10)
        {
           
            Bscore2 = 1000;
            FindObjectOfType<Highscore>().AddPoint(point2);
        }
        if (enemy_baseleft.BaseCnt >= 1)
        {
  
            Bscore3 = 10000;
            FindObjectOfType<Highscore>().AddPoint(point3);
        }
     
    }

}
