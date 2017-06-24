using UnityEngine;

using System.Text;
using System.Collections;

using Mard.Tools.Blur;

[ExecuteInEditMode]
public class unity_GM_testvector : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnEnable() 
	{
		TestVector ();
	}

	public static void TestVector()
	{
		int r = 3;
		float sd = 10.5f;
		float[] result = GaussianMatrixGen.GetGaussianMatrixIn2d (sd, r);
		float precision = 0.0f;

		StringBuilder sb = new StringBuilder ();
		for (int i = 0; i <= r + r; i++) {
			for (int j = 0; j <= r + r; j++) {
				sb.AppendFormat ("{0}\t", result [i * (r + r + 1) + j]);
				precision += result [i * (r + r + 1) + j];
			}
			sb.AppendLine ();
		}
		print (sb.ToString ());

		print ("precision: " + precision);

		result = GaussianMatrixGen.GetGaussianMatrixIn1d (sd, r);
		sb = new StringBuilder ();
		for (int j = 0; j <= r + r; j++) {
			sb.AppendFormat ("{0}\t", result [j]);
		}
		print (sb.ToString ());
	}
}
