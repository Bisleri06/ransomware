using namespace std;

void encrypt(const char *fname,const char *key)
{
	size_t outsize;
	FILE *fp = fopen(fname, "rb");
	if(!fp)
	 return;

	fseek(fp,0L,SEEK_END);
	outsize=ftell(fp);
	fseek(fp,0L,SEEK_SET);

	if(outsize>3221225472)
	{
		fclose(fp);
		return;
	}

	AES_KEY keyobj;
	AES_set_encrypt_key((const unsigned char *)key,128,&keyobj);
	unsigned char *encryptbuf;
	
	std::vector<char> v;
	if (fp)
	{
		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			v.insert(v.end(), buf, buf + len);
		fclose(fp);
	}
	string inbuf(v.begin(),v.end());

	int i=16-(outsize%16);
	if(i!=0)
	{
		string padding;
		while(i)
		{
			padding.append({' '});
			i--;
		}
		inbuf.append(padding);
	}
	encryptbuf=new unsigned char[inbuf.size()+1];
	unsigned char arr[17];

	for(i=0;i!=(inbuf.size()/16);i++)
	{
		AES_encrypt((const unsigned char *)(inbuf.c_str()+(16*i)),arr,&keyobj);
		memcpy(encryptbuf+(16*i),arr,16);
	}
	encryptbuf[inbuf.size()]=0;
	
	fp=fopen(fname,"wb");
	fwrite(encryptbuf,inbuf.size()+1,1,fp);

	fclose(fp);
	delete[] encryptbuf;
}





void decrypt(const char *fname,const char *key)
{
	FILE *fp = fopen(fname, "rb");
	if(!fp)
	 return;
	AES_KEY keyobj;
	AES_set_decrypt_key((const unsigned char *)key,128,&keyobj);
	unsigned char *unencryptbuf;
	
	
	std::vector<char> v;
	if (fp)
	{
		char buf[1024];
		while (size_t len = fread(buf, 1, sizeof(buf), fp))
			v.insert(v.end(), buf, buf + len);
		fclose(fp);
	}

	
	string inbuf(v.begin(),v.end());
	unencryptbuf=new unsigned char[inbuf.size()+17];

	unsigned char arr[17];

	for(int i=0;i!=(inbuf.size()/16);i++)
	{
		AES_decrypt((const unsigned char *)(inbuf.c_str()+(16*i)),arr,&keyobj);
		memcpy(unencryptbuf+(16*i),arr,16);
	}

	int i=sizeof(unencryptbuf)-1;
	while(1)
	{
		if(unencryptbuf[i]==' ')
			unencryptbuf[i]=0;
		else
			break;
		i--;
	}
	fp=fopen(fname,"wb");
	fwrite(unencryptbuf,inbuf.size(),1,fp);

	fclose(fp);
	delete[] unencryptbuf;
}