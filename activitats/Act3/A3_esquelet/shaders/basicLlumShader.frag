#version 330 core

in vec3 fcolor; 
out vec3 FragColor;

uniform vec3 llumAmbient;
uniform vec4 posFocusSCO;
uniform vec3 colorFocus;

uniform vec4 llumFar1;
uniform vec4 llumFar2;

uniform vec3 DirF1;
uniform vec3 DirF2;
uniform int torxesOnOff[6];
uniform vec4 posFocusTorxes[6];
uniform vec3 llumTorxes;
uniform int boolMar;

in mat4 viewMar;

in vec3 matambfs;  
in float matshinfs;
in vec3 matspecfs; 
in vec4 vSCO;
in vec3 normSCO;
in vec3 matdifffs;
in vec3 vertexMar;
in vec3 normalMar;

uniform float temps;

vec3 Ambient(vec3 llumAmbient) {
    return llumAmbient * matambfs;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdifffs * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshinfs == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinfs);
    return (matspecfs * colFocus * shine); 
}

vec3 WaveNormal(vec2 waveDirNN, float waveLength, vec3 vertex_world, float time){
        float steepness = .5;
	vec2 waveDir = normalize(waveDirNN);
	float wl = 2;
        float f = (2 * 3.14159/waveLength) * (dot(waveDir, vertex_world.xz) - 0.25 * time);

	vec3 B = vec3( 
		     1- (steepness*waveDir.x*waveDir.x * cos(f)) , 
		      waveDir.x * sin(f),
		     -steepness*waveDir.x*waveDir.y * cos(f)
			);
	vec3 T = vec3( 
		     -steepness*waveDir.x*waveDir.y * cos(f) , 
		      waveDir.y * sin(f),
		     1-steepness*waveDir.y*waveDir.y * cos(f)
			);	

	return normalize(cross(normalize(T), normalize(B)));
}



void main()
{
  vec3 NormSCOfs;
  if(boolMar == 1 && normalMar.y > 0.0){
    vec2 waveDir = vec2(0.8, 0.6);
    float waveLength = 0.3;
    vec3 normalMarSCO = normalize(viewMar * vec4(WaveNormal(waveDir, waveLength, vertexMar, temps), 0)).xyz;
  
    NormSCOfs = normalMarSCO;
  }else{
	  NormSCOfs = normalize(normSCO);

  }
	vec4 L = normalize(posFocusSCO - vSCO);
	vec3 LFar = normalize(llumFar1.xyz - vSCO.xyz);
	vec3 LFar2 = normalize(llumFar2.xyz - vSCO.xyz);
	float factorFar1 = pow(dot(DirF1, -LFar), 4.0);
	float factorFar2 = pow(dot(DirF2, -LFar2), 4.0);
    FragColor = Ambient(llumAmbient) + Difus(NormSCOfs, L.xyz, colorFocus) + Especular(NormSCOfs, L.xyz, vSCO, colorFocus) + Difus(NormSCOfs, LFar.xyz, colorFocus) * factorFar1 + Especular(NormSCOfs, LFar.xyz, vSCO, colorFocus) * factorFar1 + Difus(NormSCOfs, LFar2.xyz, colorFocus) * factorFar2 + Especular(NormSCOfs, LFar2.xyz, vSCO, colorFocus) * factorFar2;
	
    for(int i = 0; i  < 6; ++i){
      if(torxesOnOff[i] == 1){
          float d = distance(posFocusTorxes[i].xyz, vSCO.xyz);
          vec3 LTorxes = normalize(posFocusTorxes[i].xyz - vSCO.xyz);
          vec3 Espec = Especular(NormSCOfs, LTorxes, vSCO, llumTorxes);
          vec3 Dif = Difus(NormSCOfs, LTorxes, llumTorxes);
          if(d >= 3.0){
            float factorTorx = exp(-(distance(posFocusTorxes[i].xyz, vSCO.xyz)-3.0));
            FragColor += Espec * factorTorx + Dif * factorTorx; 
          } else {
            FragColor += Espec + Dif;
          }
      }

    }
	

}


