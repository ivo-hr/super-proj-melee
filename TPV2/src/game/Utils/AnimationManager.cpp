#include "AnimationManager.h"
#include "../../utils/CheckML.h"


void AnimationManager::UpdateIndex()
{
	//Restamos 1 al contador
	cont--;
	//Cuando llegue a 0...
	if (cont == 0) {
		//Aumenta el ?ndice para renderizar el siguiente sprite
		currIndex++;

		if (currentAnim->keySprite.size() == 0)
		{
			if (currIndex == currentAnim->totalSprites + currentAnim->iniSprite)
			{
				if (!currentAnim->loop)
				{
					//Si no loopea inicia la animacion de idle
					StartAnimation("idle");
				}
				else
				{
					//Si loopea reinicia la animacion
					StartAnimation(currentAnimIndex);
				}
			}
		}
		else
		{
			if (currentState == currentAnim->keySprite.size())
			{
				if (currIndex == currentAnim->totalSprites + currentAnim->iniSprite)
				{
					if (!currentAnim->loop)
					{
						//Si no loopea inicia la animacion de idle
						StartAnimation("idle");
					}
					else
					{
						//Si loopea reinicia la animacion
						StartAnimation(currentAnimIndex);
					}
				}
			}
			else
			{
				if (currIndex >= currentAnim->keySprite[currentState] + currentAnim->iniSprite)
				{
					currentState++;
					if (currentState == currentAnim->keySprite.size())
					{
						if (currentAnim->totalSprites - currentAnim->keySprite[currentState - 1] == 0)
						{
							framespSprite = currentAnim->totalFrames - currentAnim->keyFrame[currentState - 1];
						}
						else
						{
							framespSprite = (int)((currentAnim->totalFrames - currentAnim->keyFrame[currentState - 1]) /
							(currentAnim->totalSprites - currentAnim->keySprite[currentState - 1]));
						}
					}
					else
					{
						if (currentAnim->keySprite[currentState] - currentAnim->keySprite[currentState - 1] == 0)
						{
							framespSprite = currentAnim->keyFrame[currentState] - currentAnim->keyFrame[currentState - 1];
						}
						else
						{
							framespSprite = (int)((currentAnim->keyFrame[currentState] - currentAnim->keyFrame[currentState - 1]) /
							(currentAnim->keySprite[currentState] - currentAnim->keySprite[currentState - 1]));
						}
					}
					if (currIndex > currentAnim->keySprite[currentState - 1] + currentAnim->iniSprite)
					{
						currIndex = currentAnim->keySprite[currentState - 1] + currentAnim->iniSprite;
					}
				}
			}
		}

		//Reinicia el contador
		cont = framespSprite;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------

AnimationManager::AnimationManager(Entity* entity, Texture* textura, spriteSheetData data) : texture(textura), info(data)
{
	ent = entity;

	w = textura->width() / data.spritesInX;			//Sacamos la anchura de cada sprite
	h = textura->height() / data.spritesInY;		//bruh

	for (int i = 0; i < data.spritesInY; i++)
	{
		for (int j = 0; j < data.spritesInX; j++)
		{
			//Creamos el vector del cual sacaremos todos los sprites por separado
			SpriteIndex.push_back(b2Vec2(j, i));
		}
	}
	//w y h representan el tama?o de cada sprite
	//recorteSheet 
	recorteSheet = { w * data.spritesInX, h * data.spritesInY, w, h };

	//ent->GetManager()->GetScreenRatio() a 1920, 1080 esto es 3.75

	xOffset = (data.leftOffset * ent->GetWidth()) * ent->GetManager()->GetScreenRatio() / 4.7f;		//En zero (offset = 4, width = 3, tiene que dar APROX 10
	//xOffset = 10.f;
	yOffset = data.upOffset * ent->GetHeight() * ent->GetManager()->GetScreenRatio() / 4.7f;			//En zero (offset = 48, height = 3, tiene que dar APROX 115
	//yOffset = 115.f;


	//Este rect representa donde se va a renderizar la textura una vez recortada
	dest = *entity->GetHurtbox();

	dest.w += data.sizeXOffset * ent->GetWidth() * ent->GetManager()->GetScreenRatio() / 4.7f;		//En zero (offset = 28, width = 3, tiene que dar APROX 68
	//dest.w += 68;
	dest.h += data.sizeYOffset * ent->GetHeight() * ent->GetManager()->GetScreenRatio() / 4.7f;		//En zero (offset = 48, height = 3, tiene que dar APROX 115
	//dest.h += 115.f;

	//Inicializamos la animacion primera (en zero es idle)
	StartAnimation("idle");
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::update()
{

	//Actualizamos el Indice de la spritesheet si es necesario
	UpdateIndex();

	//Este rect representa donde se va a renderizar la textura una vez recortada
	SDL_Rect aux = *ent->GetHurtbox();

	//Depende de a donde est? mirando, el cuadro donde se renderiza se calcula de una manera u otra
	if (ent->GetDir() > 0) {
		dest.x = aux.x - xOffset;
		dest.y = aux.y - yOffset;

		recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };

		lookingRight = true;
	}
	else {
		dest.x = aux.x + xOffset - info.sizeXOffset * ent->GetWidth() * ent->GetManager()->GetScreenRatio() / 4.7f;
		dest.y = aux.y - yOffset;

		recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };

		lookingRight = false;
	}

}

void AnimationManager::render()
{
	if (lookingRight) {
		texture->render(recorteSheet, dest);
	}
	else {
		texture->render(recorteSheet, dest, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}

	////If debug...
	//SDL_SetRenderDrawColor(ent->GetManager()->GetSDLU()->renderer(), 128, 128, 255, 255);
	//SDL_RenderDrawRect(ent->GetManager()->GetSDLU()->renderer(), &dest);

}

void AnimationManager::render(SDL_Rect* camera)
{
	SDL_Rect aux = dest;

	aux.x -= camera->x;
	aux.x *= (ent->GetManager()->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (ent->GetManager()->GetActualHeight() / (float)camera->h);		//Se puede cambiar por width y sigue siendo la misma proporcion

	aux.w *= (ent->GetManager()->GetActualWidth() / (float)camera->w);
	aux.h *= (ent->GetManager()->GetActualHeight() / (float)camera->h);

	if (lookingRight) {
		texture->render(recorteSheet, aux);
	}
	else {
		texture->render(recorteSheet, aux, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void AnimationManager::render(SDL_Rect* camera, const Vector2D& offset)
{
	SDL_Rect aux = dest;

	aux.x -= camera->x;
	aux.x *= (ent->GetManager()->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (ent->GetManager()->GetActualHeight() / (float)camera->h);		//Se puede cambiar por width y sigue siendo la misma proporcion

	aux.w *= (ent->GetManager()->GetActualWidth() / (float)camera->w);
	aux.h *= (ent->GetManager()->GetActualHeight() / (float)camera->h);

	aux.x += offset.getX();
	aux.y += offset.getY();

	if (lookingRight) {
		texture->render(recorteSheet, aux);
	}
	else {
		texture->render(recorteSheet, aux, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void AnimationManager::StartAnimation(std::string index)
{
	currentAnimIndex = index;
	//La animacion actual cambia a ser la nueva especificada
	currentAnim = &info.animations[index];

	//El indice ser? el primero que est? en los datos
	currIndex = currentAnim->iniSprite;

	currentState = 0;

	assert(currentAnim->keySprite.size() == currentAnim->keyFrame.size());

	//Calcula cuanto debe durar cada sprite para que...
	if (currentAnim->keySprite.size() == 0)
	{
		//Acabe a la vez que el movimiento
		framespSprite = (int)(currentAnim->totalFrames / currentAnim->totalSprites);

		//Inicializamos el contador
		cont = framespSprite + currentAnim->totalFrames % currentAnim->totalSprites;
	}
	else
	{
		//El key Sprite coincida con la hitbox
		framespSprite = (int)(currentAnim->keyFrame[0] / currentAnim->keySprite[0]);

		//Inicializamos el contador
		cont = framespSprite + (currentAnim->keyFrame[0] % currentAnim->keySprite[0]);
	}
}

int AnimationManager::GetAnimationDuration()
{
	return currentAnim->totalFrames;
}



