using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//UI使うときに必要
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Choice : MonoBehaviour
{
    Button button;


   
    void Start()
    {
       
        button = GameObject.Find("Canvas/ButtonSummary/Button").GetComponent<Button>();
        //ボタンが選択された状態になる
        button.Select();
    }
    
}