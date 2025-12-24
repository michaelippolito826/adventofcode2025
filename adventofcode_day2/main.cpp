#include <curl/curl.h>
#include <iostream>
#include <string>

// Callback function to write data received from the server into a std::string
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

std::string input_fetcher(const std::string &url,
						  const std::string &session_cookie)
{
	std::string readBuffer;
	CURL *curl = curl_easy_init();
	if (curl)
	{
		CURLcode res;

		// Set the URL
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// Set the Session Cookie (Required for AoC input)
		// Format: "session=YOUR_SESSION_KEY"
		std::string cookie_header = "session=" + session_cookie;
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie_header.c_str());

		// Set up the write callback to capture the response
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		// Perform the request
		res = curl_easy_perform(curl);

		// Check for errors
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
					  << std::endl;
		}

		// Clean up
		curl_easy_cleanup(curl);
	}
	return readBuffer;
}

// Helper for mathematical floor division
long long floor_div(long long a, long long b)
{
	long long res = a / b;
	if (a % b < 0 && b > 0)
		res--;
	return res;
}

long long process_data(std::string dataInput)
{
	long long password = 0;
	long long dial_pos =
		50; // Use long long to prevent overflow during intermediate calculations
	size_t pos_start = 0;
	size_t pos_end = 0;
	std::string delimiter = "\n";
	std::string token = "";
	long long iToken;

	while (pos_start < dataInput.length())
	{
		pos_end = dataInput.find(delimiter, pos_start);
		if (pos_end == std::string::npos)
		{
			pos_end = dataInput.length();
		}

		token = dataInput.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delimiter.length();

		if (token.empty())
			continue;

		// Left = negative, right = positive
		int direction = (token[0] == 'R') ? 1 : -1;
		// Removes L or R
		token.erase(0, 1);

		try
		{
			iToken = std::stoll(token);
		}
		catch (const std::exception &e)
		{
			std::cerr << "NaN:" << e.what() << std::endl;
			continue;
		}

		if (direction == 1) // Right (Positive)
		{
			long long prev_pos = dial_pos;
			dial_pos += iToken;

			// Count multiples of 100 in (prev_pos, dial_pos]
			long long count = floor_div(dial_pos, 100) - floor_div(prev_pos, 100);
			password += count;
		}
		else // Left (Negative)
		{
			long long prev_pos = dial_pos;
			dial_pos -= iToken; // iToken is magnitude

			// Count multiples of 100 in [dial_pos, prev_pos)
			// Formula: floor((prev_pos - 1) / 100) - floor((dial_pos - 1) / 100)
			long long count =
				floor_div(prev_pos - 1, 100) - floor_div(dial_pos - 1, 100);
			password += count;
		}

		// Normalize dial_pos to [0, 99] for the next step
		dial_pos %= 100;
		if (dial_pos < 0)
			dial_pos += 100;
	}
	return password;
}

int main()
{
	const std::string url = "https://adventofcode.com/2025/day/1/input";

	// TODO: Replace with your actual session cookie from your browser
	// (Open DevTools -> Application -> Cookies -> adventofcode.com -> session)
	const std::string session_cookie =
		"53616c7465645f5f8cac7da1f63672b757dbd6ad9897db04b579f7af78fe2e8adf2261e1"
		"aad805f26cf29cf6bf12d3a1d8184d8d5b319bf14ddc513ccbdf7444";

	std::string dataInput = input_fetcher(url, session_cookie);
	long long password = process_data(dataInput);
	std::cout << password << std::endl;

	return 0;
}
