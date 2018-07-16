#GL_VARS
	depthMask on
	writeZ off
	blend on
	colorMask R A
	cull on
	cullMode back // front

#VERTEX
	attribute float3 Position;
	attribute float2 UV;
			
	varying float2 uv;
	
	void main() {
		uv = Position.xy * 0.5 + 0.5; 
		gl_Position = vec4 (Position.xy, 0, 1);
	}
	
#FRAGMENT
	uniform sampler2D MainTex;
	varying float2 uv;
	
	void main() 
	{
		vec4 c = texture2D (MainTex, uv);
		//if (c.a<0.5) discard;
		gl_FragColor = c;
		//gl_FragColor.a = 0.5;
		//gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D (MainTex, uv.xy).r);
	}