#include "gucc/guccmain.h"
#include "gucc/pl.h"

namespace guccstd
{
	class gucc_state
	{
	public:
		std::string filepath = "gucc/main.gucc";
		bool cmd = false;
		gucc_state(std::string file = "gucc/main.gucc", bool cmdline = false)
		{
			filepath = file;
			cmd = cmdline;
		}

		operator std::string()
		{
			return "\""+filepath+"\"";
		}

		void init()
		{
			guccmain(filepath, cmd);
		}

		void init_to_point(unsigned long long int index = SIZE_MAX)
		{
			std::string GuccAsString;

			std::string line;
			std::ifstream guccfile(filepath);

			while (getline(guccfile, line))
			{
				GuccAsString+=line;
			}

			std::vector<std::string> guccvector = psplit(GuccAsString, '>');

			std::string cmd;

			for (int i = 0; i < guccvector.size(); i++)
			{
				if (index > i)
				{
					cmd+=guccvector[i]+'>';
				}
			}
			Main.intline(cmd, 0);

		}

		void GuccExecString(std::string cmd)
		{
			Main.intline(cmd, 0);
		}

		std::string GetGuccValue(std::string varname)
		{
			for (int i = 0; i < vars.size(); i++) {
				if (vars[i].name == varname) {
					return vars[i].value;
				}
			}
			return "nullptr";

		}	

		Auto autoGetGuccValue(std::string varname)
		{
			Auto ret = new std::string("nullptr");
			for (int i = 0; i < vars.size(); i++) {
				if (vars[i].name == varname) {
					ret = new std::string(vars[i].value);
					return ret;
				}
			}
			return ret;

		}	

		void CallGuccFunction(std::string s, std::vector<std::string> arguments = {})
		{
			for (int i = 0; i < functions.size(); i++) {
				if (functions[i].name == s) {
					for (int c = 0; c < arguments.size(); c++)
					{
						string f = "redef>"+functions[i].arguments[c];
						cout << f << endl;
						Main.intline(f, -1);
					}
					Main.intline(functions[i].exec, -1);
				}
			}
		}

		void SetGuccValue(std::string name, std::string value)
		{
			Main.intline("redef>"+name+">"+value, 0);
		}

	};


	gucc_state* CreateGuccState(std::string filename = "gucc/main.gucc", bool cmdline = false)
	{
		gucc_state* returngucc = new gucc_state(filename, cmdline);
		returngucc->init();
		return returngucc;
	}

	void CloseGuccState(gucc_state* gucc)
	{
		delete gucc;
	}
}
