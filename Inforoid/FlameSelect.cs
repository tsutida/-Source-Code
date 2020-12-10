using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEngine.Networking;
using Makina;
using UnityEngine.UI;

public class FlameSelect : MonoBehaviour
{

    public static FlameSelect Instance;

    private GameObject[] Flames = new GameObject[4];
    public static int resflamecheck;
    public static bool checkFlame { get; private set; }
    public static bool checkEndResflame;
    // Start is called before the first frame update
    void Start()
    {
        Instance = this;
        Flames[0] = GameObject.FindWithTag("FlameImage01");
        Flames[1] = GameObject.FindWithTag("FlameImage02");
        Flames[2] = GameObject.FindWithTag("FlameImage01");
        Flames[3] = GameObject.FindWithTag("FlameImage02");
        checkFlame = false;
        checkEndResflame = false;
        resflamecheck = 0;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public IEnumerator SelectMode() 
    {
        Debug.Log("フレーム選択開始");
        yield return GameManager.Instance.CeVIOSpeak("フレームを選択してね");
        checkFlame = true;
        Debug.Log("できてる");
        yield return new WaitUntil(() => checkEndResflame == true);
        Flames[resflamecheck].transform.localScale = new Vector3(1, 1, 1);//フレーム画像（背景）表示

    }

    /*public void Speakuser()
    {
         int i = 0;
         string[] flameText1 = new string[] { "いち", "に", "さん", "よん" };
         string[] flameText2 = new string[] { "ひとつ","ふたつ", "みっつ", "よっつ" };
         string[] flameText3 = new string[] { "いちばん", "にばん", "さんばん","よんばん" };
         string[][] flamesText = new string[][] {flameText1,flameText2,flameText3};
     
        foreach (var hogehoge in flamesText)
        {
            //Debug.Log(hogehoge[0]);
            if(text == hogehoge[0])
             {
                 resflamecheck = i;
                 checkEndResflame = true;
                 Debug.Log(hogehoge);
             }
        }
    }*/
}
