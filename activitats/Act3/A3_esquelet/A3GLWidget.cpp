#include "A3GLWidget.h"

#include <iostream>

A3GLWidget::A3GLWidget (QWidget* parent) : MyGLWidget(parent)
{
}

A3GLWidget::~A3GLWidget()
{
}

void A3GLWidget::onFocusMoved(int hour){
	makeCurrent();
	if(hour < horaAct){
		if(degreeSol > 0.0){
				horaAct--;
				degreeSol -= (90.0/6);
				posFocus.x = 40 * glm::cos(glm::radians(degreeSol));
				posFocus.y = 40 * glm::sin(glm::radians(degreeSol));
			}
	}else if(hour > horaAct){
		if(degreeSol < 180.0){
				degreeSol += (90.0/6);
				horaAct++;
				posFocus.x = 40 * glm::cos(glm::radians(degreeSol));
				posFocus.y = 40 * glm::sin(glm::radians(degreeSol));
			}
	}
	update();
}


void A3GLWidget::onTorxaOne(bool OnOff){
	makeCurrent();
	int boolres;
	if(OnOff) boolres = 1;
	else boolres = 0;
	torxesOnOff[0] = boolres;
	update();
}


void A3GLWidget::onTorxaTwo(bool OnOff){
	makeCurrent();
	int boolres;
	if(OnOff) boolres = 1;
	else boolres = 0;
	torxesOnOff[1] = boolres;
	update();
}

void A3GLWidget::onTorxaThree(bool OnOff){
	makeCurrent();
	int boolres;
	if(OnOff) boolres = 1;
	else boolres = 0;
	torxesOnOff[2] = boolres;
	update();
}

void A3GLWidget::onTorxaFour(bool OnOff){
	makeCurrent();
	int boolres;
	if(OnOff) boolres = 1;
	else boolres = 0;
	torxesOnOff[3] = boolres;
	update();
}

void A3GLWidget::onTorxaFive(bool OnOff){
	makeCurrent();
	int boolres;
	if(OnOff) boolres = 1;
	else boolres = 0;
	torxesOnOff[4] = boolres;
	update();
}

void A3GLWidget::onTorxaSix(bool OnOff){
	makeCurrent();
	int boolres;
	if(OnOff) boolres = 1;
	else boolres = 0;
	torxesOnOff[5] = boolres;
	update();
}


void A3GLWidget::redBar(int color){
	makeCurrent();
	colorTorxes.x = color/255.0;
	update();
}

void A3GLWidget::greenBar(int color){
	makeCurrent();
	colorTorxes.y = color/255.0;
	update();
}

void A3GLWidget::blueBar(int color){
	makeCurrent();
	colorTorxes.z = color/255.0;
	update();
}

void A3GLWidget::iniEscena ()
{
	creaBuffersModels();
	
	VaixellPos = glm::vec3(-10, 0.0, 0.0);
	posFocus = glm::vec3(0, 40, 0);
	degreeSol = 90;
	horaAct = 14;
	boolMar = 0;
	
	for(unsigned int i = 0; i < NUM_TORXES; ++i){
		torxesOnOff[i] = 0;
	}
	colorTorxes = glm::vec3(0, 0, 0);
	makeCurrent();
	emit(signalFocusMovedKeyboard(horaAct));
	update();
	//--------------------------------------------------
	centreEsc = glm::vec3 (0, 0, 0);
	radiEsc = 20;// Tret de la màniga :-P

}


void A3GLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

	// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
	// useu els paràmetres que considereu (els que hi ha són els de per defecte)
	//  glViewport (0, 0, ample, alt);
	//Pasem el temps com a uniform al fragment shader
	glUniform1f(tempsLoc, temps);
	projectTransform ();
	viewTransform ();
	// Esborrem el frame-buffer i el depth-buffer
	//No estem pintant el mar
	boolMar = 0;
	//Passem com a uniform al fragment shader si estem pintant el mar o no
	glUniform1i(boolMarLoc, boolMar);

	glClearColor(0.8f, 0.8f, 1.0f, 1.f);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::vec3 llumAmbient = glm::vec3(0.1, 0.1, 0.1);
	glUniform3fv(llumAmbientloc, 1, &llumAmbient[0]);
	
	glm::vec4 posFocusSCO =  View * glm::vec4(posFocus, 1.0);	//View * posFocus en SCO
	glUniform4fv(posFocusLoc, 1, &posFocusSCO[0]);

	glUniform1iv(torxesOnOffLoc, 6, torxesOnOff);
	glm::vec3 colorFocus = glm::vec3(0.6, 0.6, 0.6);
	glUniform3fv(colorFocusLoc, 1, &colorFocus[0]);
	glm::mat3 normalMatrix;
	glm::mat4 TGnormal;
	normalMatrix = inverse(transpose(glm::mat3(View  * Vaixell_TG)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
	glm::mat4 TGFar1, TGFar2;
	TGFar1 = modelTransformFar1();
	TGFar2 = modelTransformFar2();
	glm::vec3 DirF1 = View * TGFar1 * glm::vec4(glm::vec3(0.363,4.695,0.357),1.0) - View * TGFar2 * glm::vec4(glm::vec3(-0.357,4.695,-0.348), 1.0);
	glm::vec3 DirF2 = View * TGFar2 * glm::vec4(glm::vec3(-0.357,4.695,-0.348), 1.0) -  View * TGFar1 * glm::vec4(glm::vec3(0.363,4.695,0.357), 1.0);
	glUniform3fv(DirF1loc, 1, &DirF1[0]);
	glUniform3fv(DirF2loc, 1, &DirF2[0]);


	//ColorTorxes
	//glm::vec3 colorTorxes = glm::vec3(1, 0, 1);
	glUniform3fv(llumTorxesLoc, 1, &colorTorxes[0]);

	//bool torxes

	/*
		glm::vec3 llumDifusa = glm::vec3(0.6, 0.6, 0.6);
		glUniform3fv(llumDiffuseloc, 1, &llumDifusa[0]);
	*/

	//--------------------------------------------------------
	// Activem el VAO per a pintar el vaixell
	glBindVertexArray (VAO_models[VAIXELL]);
	// Transformació geometrica
	modelTransformVaixell();
	//Posicions torxes 
	glm::vec4 torxesPos[NUM_TORXES];
	torxesPos[0] = View * Vaixell_TG * glm::vec4(glm::vec3(-7.39, 1.95,-6.68), 1.0);
	torxesPos[1] = View * Vaixell_TG * glm::vec4(glm::vec3(-9.95, 1.95,-0.56), 1.0);
	torxesPos[2] = View * Vaixell_TG * glm::vec4(glm::vec3(-7.47, 1.95, 5.64), 1.0);
	torxesPos[3] = View * Vaixell_TG * glm::vec4(glm::vec3(4.38, 1.95, 5.26), 1.0);
	torxesPos[4] = View * Vaixell_TG * glm::vec4(glm::vec3(6.68, 1.95, 0.38), 1.0);
	torxesPos[5] = View * Vaixell_TG * glm::vec4(glm::vec3(4.15, 1.95,-6.97), 1.0);
	glUniform4fv(posFocusTorxesLoc, NUM_TORXES, &torxesPos[0][0]); 
	
	// pintem el vaixell
	glDrawArrays(GL_TRIANGLES, 0, models[VAIXELL].faces().size()*3);
	//--------------------------------------------------------
	// Activem el VAO per a pintar el moll
	glBindVertexArray (VAO_models[MOLL]);
	// pintem les espelmes, cadascuna amb el seu transform
	TGnormal = modelTransformMoll();
	normalMatrix = inverse(transpose(glm::mat3(View  * TGnormal)));
	glDrawArrays(GL_TRIANGLES, 0, models[MOLL].faces().size()*3);
	//--------------------------------------------------------
	// Activem el VAO per a pintar el far (primera part)
	glBindVertexArray (VAO_models[FAR_1]);
	// pintem les espelmes, cadascuna amb el seu transform
	TGFar1 = modelTransformFar1();
	glm::vec3 llumFar1 = View * TGFar1 * glm::vec4(glm::vec3(0.363,4.695,0.357), 1.0);
	glUniform4fv(llumFar1loc, 1, &llumFar1[0]);
	normalMatrix = inverse(transpose(glm::mat3(View  * TGFar1)));
	glDrawArrays(GL_TRIANGLES, 0, models[FAR_1].faces().size()*3);
	//--------------------------------------------------------
	// Activem el VAO per a pintar el far (segona part)
	glBindVertexArray (VAO_models[FAR_2]);
	// pintem les espelmes, cadascuna amb el seu transform
	TGFar2 = modelTransformFar2();
	glm::vec3 llumFar2 = View * TGFar2 * glm::vec4(glm::vec3(-0.357,4.695,-0.348), 1.0);
	glUniform4fv(llumFar2loc, 1, &llumFar2[0]);
	normalMatrix = inverse(transpose(glm::mat3(View  * TGFar2)));

	glDrawArrays(GL_TRIANGLES, 0, models[FAR_2].faces().size()*3);

	//------------------------------------------------------------
	//Pintem terra
	glBindVertexArray (VAO_Terra);
	TGnormal = modelTransformTerra();
	normalMatrix = inverse(transpose(glm::mat3(View  * TGnormal)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Pintem Mar
	boolMar = 1;
	glUniform1i(boolMarLoc, boolMar);
	glBindVertexArray (VAO_Mar);
	TGnormal = modelTransformMar();
	normalMatrix = inverse(transpose(glm::mat3(View  * TGnormal)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//--------------------------------------------------------

	glBindVertexArray(0);
}

glm::mat4 A3GLWidget::modelTransformFar1()
{
	// Codi per a la TG necessària
	glm::mat4 TG = glm::mat4(1.0f);
	TG = glm::translate(TG, glm::vec3(2, 1, 4));
	TG = glm::scale(TG, glm::vec3(escalaModels[FAR_1]) );
	TG = glm::translate(TG, -centreBaseModels[FAR_1]);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
	return TG;
}

glm::mat4 A3GLWidget::modelTransformFar2()
{
	// Codi per a la TG necessària
	Far2_TG = glm::mat4(1.0f);
	Far2_TG = glm::translate(Far2_TG, glm::vec3(2, 1, 4));
	Far2_TG = glm::rotate(Far2_TG, glm::radians(degreeFar), glm::vec3(0, 1, 0));
	Far2_TG = glm::scale(Far2_TG, glm::vec3(escalaModels[FAR_1]) );
	Far2_TG = glm::translate(Far2_TG, -centreBaseModels[FAR_1]);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Far2_TG[0][0]);
	return Far2_TG;
}


glm::mat4 A3GLWidget::modelTransformVaixell()
{
	// Codi per a la TG necessària
	Vaixell_TG = glm::mat4(1.0f);
	Vaixell_TG = glm::translate(Vaixell_TG, VaixellPos);
	Vaixell_TG = glm::scale(Vaixell_TG, glm::vec3(escalaModels[VAIXELL]) );
	Vaixell_TG = glm::translate(Vaixell_TG,
		                -glm::vec3(centreBaseModels[VAIXELL].x,
		                           0,
		                           centreBaseModels[VAIXELL].z)); // el vaixell ja esta a la línia de flotació....no toquem en alçada
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Vaixell_TG[0][0]);
	return Vaixell_TG;
}



void A3GLWidget::keyPressEvent(QKeyEvent* event)
{
	makeCurrent();

	switch (event->key()) {

		case Qt::Key_S: {
			VaixellPos[2]+=1;break;
		}
		case Qt::Key_W: {
			VaixellPos[2]-=1;break;
		}
		case Qt::Key_Up:{
			if(degreeSol < 180.0){
				degreeSol += (90.0/6);
				horaAct++;
				posFocus.x = 40 * glm::cos(glm::radians(degreeSol));
				posFocus.y = 40 * glm::sin(glm::radians(degreeSol));
				emit(signalFocusMovedKeyboard(horaAct));
			}
			break;
		}
		case Qt::Key_Down:{
			makeCurrent();
			if(degreeSol > 0.0){
				horaAct--;
				degreeSol -= (90.0/6);
				posFocus.x = 40 * glm::cos(glm::radians(degreeSol));
				posFocus.y = 40 * glm::sin(glm::radians(degreeSol));
				emit(signalFocusMovedKeyboard(horaAct));
			}
			break;
		}
		case Qt::Key_D:{
			degreeFar += 5;
			break;
		}
		case Qt::Key_A:{
			degreeFar -= 5;
			break;
		}
		default: event->ignore(); break;
	}
	update();
}


void A3GLWidget::carregaShaders()
{
	// Creem els shaders per al fragment shader i el vertex shader
	QOpenGLShader fs (QOpenGLShader::Fragment, this);
	QOpenGLShader vs (QOpenGLShader::Vertex, this);
	// Carreguem el codi dels fitxers i els compilem
	fs.compileSourceFile("./shaders/basicLlumShader.frag");
	vs.compileSourceFile("./shaders/basicLlumShader.vert");
	// Creem el program
	program = new QOpenGLShaderProgram(this);
	// Li afegim els shaders corresponents
	program->addShader(&fs);
	program->addShader(&vs);
	// Linkem el program
	program->link();
	// Indiquem que aquest és el program que volem usar
	program->bind();

	// Obtenim identificador per a l'atribut “vertex” del vertex shader
	vertexLoc = glGetAttribLocation (program->programId(), "vertex");
	// Obtenim identificador per a l'atribut “normal” del vertex shader
	normalLoc = glGetAttribLocation (program->programId(), "normal");
	// Obtenim identificador per a l'atribut “matamb” del vertex shader
	matambLoc = glGetAttribLocation (program->programId(), "matamb");
	// Obtenim identificador per a l'atribut “matdiff” del vertex shader
	matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
	// Obtenim identificador per a l'atribut “matspec” del vertex shader
	matspecLoc = glGetAttribLocation (program->programId(), "matspec");
	// Obtenim identificador per a l'atribut “matshin” del vertex shader
	matshinLoc = glGetAttribLocation (program->programId(), "matshin");

	// Demanem identificadors per als uniforms del vertex shader
	transLoc = glGetUniformLocation (program->programId(), "TG");
	projLoc = glGetUniformLocation (program->programId(), "proj");
	viewLoc = glGetUniformLocation (program->programId(), "view");
	normalMatrixLoc = glGetUniformLocation (program->programId(), "normalMatrix");
	llumAmbientloc = glGetUniformLocation (program->programId(), "llumAmbient");
	posFocusLoc = glGetUniformLocation (program->programId(), "posFocusSCO");
	colorFocusLoc = glGetUniformLocation (program->programId(), "colorFocus");
	llumFar1loc = glGetUniformLocation (program->programId(), "llumFar1");
	llumFar2loc = glGetUniformLocation (program->programId(), "llumFar2");

	DirF1loc = glGetUniformLocation (program->programId(), "DirF1");
	DirF2loc = glGetUniformLocation (program->programId(), "DirF2");
	llumTorxesLoc = glGetUniformLocation (program->programId(), "llumTorxes");
	posFocusTorxesLoc = glGetUniformLocation (program->programId(), "posFocusTorxes");
	torxesOnOffLoc = glGetUniformLocation (program->programId(), "torxesOnOff");
	boolMarLoc = glGetUniformLocation (program->programId(), "boolMar");
	tempsLoc = glGetUniformLocation (program->programId(), "temps");
	// Bloc d'uniforms
	// .... Enjoy !
}
