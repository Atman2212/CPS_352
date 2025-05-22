using UnityEngine;
using UnityEngine.SceneManagement;

public class LevelController : MonoBehaviour
{
    private static int _nextLevelIndex = 1;
    private Enemy[] _enemies;
    private bool _levelCompleted;

    private void OnEnable()
    {
        _enemies = FindObjectsOfType<Enemy>();
    }

    // Update is called once per frame
    void Update()
    {
        if (_levelCompleted) 
        {
            return;
        }

        foreach (Enemy enemy in _enemies)
        {
            if (enemy != null)
            {
                return;
            }
        }

        Debug.Log("You killed all enemies");

        _levelCompleted = true;

        _nextLevelIndex++;
        string nextLevelName = "Level" + _nextLevelIndex;

        if (Application.CanStreamedLevelBeLoaded(nextLevelName))
        {
            SceneManager.LoadScene(nextLevelName);
        } 
        else 
        {
            SceneManager.LoadScene("VictoryScene");
        }
    }
}
