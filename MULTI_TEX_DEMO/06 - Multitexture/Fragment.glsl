uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
	gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
	gl_FragColor += texture2D(tex1, gl_TexCoord[0].st);
}