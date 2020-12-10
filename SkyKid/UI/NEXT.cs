using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class NEXT : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
            Invoke("ChangeScene", 3.0f);
    }

    void ChangeScene()
    {
        SceneManager.LoadScene("TitleScenes");
        Highscore.score = 0;
        Player.life = 4;

    }
}
