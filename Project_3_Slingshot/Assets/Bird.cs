using UnityEngine;
using UnityEngine.SceneManagement;

public class Bird : MonoBehaviour
{
    private Vector3 _initialPosition;
    private bool _birdWasLaunched;
    private float _timeSittingAround;
    private TrailRenderer _trailRenderer;

    [SerializeField] private float _launchPower = 500;

    private void Awake()
    {
        _initialPosition = transform.position;
        _trailRenderer = GetComponent<TrailRenderer>();
        _trailRenderer.enabled = false;
    }

    private void Update()
    {
        
        GetComponent<LineRenderer>().SetPosition(0, transform.position);
        GetComponent<LineRenderer>().SetPosition(1, _initialPosition);

        if (_birdWasLaunched &&
        GetComponent<Rigidbody2D>().linearVelocity.magnitude <= 0.1) 
        {
            _timeSittingAround += Time.deltaTime;
        }

        if (transform.position.y > 20 ||
            transform.position.y < -20 ||
            transform.position.x > 20 ||
            transform.position.x < -20 ||
            _timeSittingAround > 3) 
        {
            string currentSceneName = SceneManager.GetActiveScene().name;
            SceneManager.LoadScene(currentSceneName);
        }
    }
    private void OnMouseDown()
    {
        GetComponent<SpriteRenderer>().color = Color.red;
        GetComponent<LineRenderer>().enabled = true;
    }

    private void OnMouseUp()
    {
        GetComponent<SpriteRenderer>().color = Color.white;

        Vector2 directionToInitialPosition = _initialPosition - transform.position;

        GetComponent<Rigidbody2D>().AddForce(directionToInitialPosition * _launchPower);
        GetComponent<Rigidbody2D>().gravityScale = 1;
        _birdWasLaunched = true;

        GetComponent<LineRenderer>().enabled = false;
        
        _trailRenderer.enabled = true;
    }

    private void OnMouseDrag()
    {
        Vector3 newPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        transform.position = new Vector3(newPosition.x, newPosition.y);
    }
}
