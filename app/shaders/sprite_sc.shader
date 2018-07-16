#VERTEX
	attribute float4 Position;

	uniform float4  pssz;
	uniform float4  mudUVXX;

	varying float2 UV;
	void main() 
	{
		UV = Position.xy * mudUVXX.xy + mudUVXX.zw;
		gl_Position = vec4 (pssz.xy * 2.0 - 1.0 + Position.xy * pssz.zw * 2.0, 0.0, 1.0);
	}

#FRAGMENT
	uniform sampler2D MainTex;
	uniform fix4 MainColor;
	varying float2 UV;

	void main() 
	{
		gl_FragColor = MainColor * texture2D (MainTex, UV);
	}