/*
Copyright (C) 2008 Grame

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include "JackAudioAdapter.h"
#include "JackLibSampleRateResampler.h"
#include "JackTime.h"  
#include <stdio.h>

namespace Jack
{

#ifdef JACK_MONITOR

    void MeasureTable::Write(int time1, int time2, float r1, float r2, int pos1, int pos2)
    {
        int pos = (++fCount) % TABLE_MAX;
        fTable[pos].time1 = time1;
        fTable[pos].time2 = time2;
        fTable[pos].r1 = r1;
        fTable[pos].r2 = r2;
        fTable[pos].pos1 = pos1;
        fTable[pos].pos2 = pos2;
    }

    void MeasureTable::Save(unsigned int fHostBufferSize, unsigned int fHostSampleRate, unsigned int fAdaptedSampleRate, unsigned int fAdaptedBufferSize)
    {
        char buffer[1024];
        FILE* file = fopen("JackAudioAdapter.log", "w");

        int max = (fCount) % TABLE_MAX - 1;
        for (int i = 1; i < max; i++)
        {
            fprintf(file, "%d \t %d \t %d  \t %f \t %f \t %d \t %d \n",
                    fTable[i].delta, fTable[i+1].time1 - fTable[i].time1,
                    fTable[i+1].time2 - fTable[i].time2,
                    fTable[i].r1, fTable[i].r2, fTable[i].pos1, fTable[i].pos2);
        }
        fclose(file);

        /* No used for now
        // Adapter timing 1
        file = fopen("AdapterTiming1.plot", "w");
        fprintf(file, "set multiplot\n");
        fprintf(file, "set grid\n");
        fprintf(file, "set title \"Audio adapter timing: host [rate = %.1f kHz buffer = %d frames] adapter [rate = %.1f kHz buffer = %d frames] \"\n"
            ,float(fHostSampleRate)/1000.f, fHostBufferSize, float(fAdaptedSampleRate)/1000.f, fAdaptedBufferSize);
        fprintf(file, "set xlabel \"audio cycles\"\n");
        fprintf(file, "set ylabel \"frames\"\n");
        fprintf(file, "plot ");
        sprintf(buffer, "\"JackAudioAdapter.log\" using 2 title \"Consumer interrupt period\" with lines,");
        fprintf(file, buffer);
        sprintf(buffer, "\"JackAudioAdapter.log\" using 3 title \"Producer interrupt period\" with lines");
        fprintf(file, buffer);
        
        fprintf(file, "\n unset multiplot\n");  
        fprintf(file, "set output 'AdapterTiming1.pdf\n");
        fprintf(file, "set terminal pdf\n");
        
        fprintf(file, "set multiplot\n");
        fprintf(file, "set grid\n");
        fprintf(file, "set title \"Audio adapter timing\"\n");
        fprintf(file, "set xlabel \"audio cycles\"\n");
        fprintf(file, "set ylabel \"frames\"\n");
        fprintf(file, "plot ");
        sprintf(buffer, "\"JackAudioAdapter.log\" using 2 title \"Consumer interrupt period\" with lines,");
        fprintf(file, buffer);
        sprintf(buffer, "\"JackAudioAdapter.log\" using 3 title \"Producer interrupt period\" with lines");
        fprintf(file, buffer);
        
        fclose(file);
        */

        // Adapter timing 2
        file = fopen("AdapterTiming2.plot", "w");
        fprintf(file, "set multiplot\n");
        fprintf(file, "set grid\n");
        fprintf(file, "set title \"Audio adapter timing: host [rate = %.1f kHz buffer = %d frames] adapter [rate = %.1f kHz buffer = %d frames] \"\n"
            ,float(fHostSampleRate)/1000.f, fHostBufferSize, float(fAdaptedSampleRate)/1000.f, fAdaptedBufferSize);
        fprintf(file, "set xlabel \"audio cycles\"\n");
        fprintf(file, "set ylabel \"resampling ratio\"\n");
        fprintf(file, "plot ");
        sprintf(buffer, "\"JackAudioAdapter.log\" using 4 title \"Ratio 1\" with lines,");
        fprintf(file, buffer);
        sprintf(buffer, "\"JackAudioAdapter.log\" using 5 title \"Ratio 2\" with lines");
        fprintf(file, buffer);
        
        fprintf(file, "\n unset multiplot\n");  
        fprintf(file, "set output 'AdapterTiming2.pdf\n");
        fprintf(file, "set terminal pdf\n");
        
        fprintf(file, "set multiplot\n");
        fprintf(file, "set grid\n");
        fprintf(file, "set title \"Audio adapter timing\"\n");
        fprintf(file, "set xlabel \"audio cycles\"\n");
        fprintf(file, "set ylabel \"resampling ratio\"\n");
        fprintf(file, "plot ");
        sprintf(buffer, "\"JackAudioAdapter.log\" using 4 title \"Ratio 1\" with lines,");
        fprintf(file, buffer);
        sprintf(buffer, "\"JackAudioAdapter.log\" using 5 title \"Ratio 2\" with lines");
        fprintf(file, buffer);
        
        fclose(file);

        // Adapter timing 3
        file = fopen("AdapterTiming3.plot", "w");
        fprintf(file, "set multiplot\n");
        fprintf(file, "set grid\n");
        fprintf(file, "set title \"Audio adapter timing: host [rate = %.1f kHz buffer = %d frames] adapter [rate = %.1f kHz buffer = %d frames] \"\n"
            ,float(fHostSampleRate)/1000.f, fHostBufferSize, float(fAdaptedSampleRate)/1000.f, fAdaptedBufferSize);
         fprintf(file, "set xlabel \"audio cycles\"\n");
        fprintf(file, "set ylabel \"frames\"\n");
        fprintf(file, "plot ");
        sprintf(buffer, "\"JackAudioAdapter.log\" using 6 title \"Frames position in consumer ringbuffer\" with lines,");
        fprintf(file, buffer);
        sprintf(buffer, "\"JackAudioAdapter.log\" using 7 title \"Frames position in producer ringbuffer\" with lines");
        fprintf(file, buffer);
        
        fprintf(file, "\n unset multiplot\n");  
        fprintf(file, "set output 'AdapterTiming3.pdf\n");
        fprintf(file, "set terminal pdf\n");
        
        fprintf(file, "set multiplot\n");
        fprintf(file, "set grid\n");
        fprintf(file, "set title \"Audio adapter timing\"\n");
        fprintf(file, "set xlabel \"audio cycles\"\n");
        fprintf(file, "set ylabel \"frames\"\n");
        fprintf(file, "plot ");
        sprintf(buffer, "\"JackAudioAdapter.log\" using 6 title \"Frames position in consumer ringbuffer\" with lines,");
        fprintf(file, buffer);
        sprintf(buffer, "\"JackAudioAdapter.log\" using 7 title \"Frames position in producer ringbuffer\" with lines");
        fprintf(file, buffer);
        
        fclose(file);
    }

#endif
        
    void JackAudioAdapterInterface::ResetRingBuffers()
    {
        if (fRingbufferSize == 0) {
            fRingbufferCurSize *=2;
            if (fRingbufferCurSize > DEFAULT_RB_SIZE) 
                fRingbufferCurSize = DEFAULT_RB_SIZE;
        } 
        
        for (int i = 0; i < fCaptureChannels; i++)
            fCaptureRingBuffer[i]->Reset(fRingbufferCurSize);
        for (int i = 0; i < fPlaybackChannels; i++)
            fPlaybackRingBuffer[i]->Reset(fRingbufferCurSize);
    }

     void JackAudioAdapterInterface::Reset()
    {
        ResetRingBuffers();
        fRunning = false;
    }

    void JackAudioAdapterInterface::Create()
    {
        //ringbuffers
        fCaptureRingBuffer = new JackResampler*[fCaptureChannels];
        fPlaybackRingBuffer = new JackResampler*[fPlaybackChannels];
        
        fRingbufferCurSize = (fRingbufferSize == 0) ? DEFAULT_ADAPTATIVE_SIZE : DEFAULT_RB_SIZE;
        
        if (fRingbufferSize == 0) {
            jack_info("Ringbuffer automatic adaptative mode");
        }
        
        for (int i = 0; i < fCaptureChannels; i++ ) {
            fCaptureRingBuffer[i] = new JackLibSampleRateResampler(fQuality);
            fCaptureRingBuffer[i]->Reset(fRingbufferCurSize);
        }
        for (int i = 0; i < fPlaybackChannels; i++ ) {
            fPlaybackRingBuffer[i] = new JackLibSampleRateResampler(fQuality);
            fPlaybackRingBuffer[i]->Reset(fRingbufferCurSize);
        }
     
        if (fCaptureChannels > 0)
            jack_log("ReadSpace = %ld", fCaptureRingBuffer[0]->ReadSpace());
        if (fPlaybackChannels > 0)
            jack_log("WriteSpace = %ld", fPlaybackRingBuffer[0]->WriteSpace());
    }

    void JackAudioAdapterInterface::Destroy()
    {
        for (int i = 0; i < fCaptureChannels; i++ )
            delete ( fCaptureRingBuffer[i] );
        for (int i = 0; i < fPlaybackChannels; i++ )
            delete ( fPlaybackRingBuffer[i] );

        delete[] fCaptureRingBuffer;
        delete[] fPlaybackRingBuffer;
    }
    
    int JackAudioAdapterInterface::PushAndPull(float** inputBuffer, float** outputBuffer, unsigned int inNumberFrames)
    {
        bool failure = false;
        fRunning = true;
        
        /*
        Finer estimation of the position in the ringbuffer ??
        int delta_frames = (int)(float(long(GetMicroSeconds() - fPullAndPushTime)) * float(fAdaptedSampleRate)) / 1000000.f;
        double ratio = fPIControler.GetRatio(fCaptureRingBuffer[0]->GetOffset() - delta_frames);
        */
         
        double ratio = fPIControler.GetRatio(fCaptureRingBuffer[0]->GetOffset());
    
        // Push/pull from ringbuffer
        for (int i = 0; i < fCaptureChannels; i++) {
            fCaptureRingBuffer[i]->SetRatio(ratio);
            if (fCaptureRingBuffer[i]->WriteResample(inputBuffer[i], inNumberFrames) < inNumberFrames)
                failure = true;
        }

        for (int i = 0; i < fPlaybackChannels; i++) {
            fPlaybackRingBuffer[i]->SetRatio(1/ratio);
            if (fPlaybackRingBuffer[i]->ReadResample(outputBuffer[i], inNumberFrames) < inNumberFrames)
                 failure = true;
        }

    #ifdef JACK_MONITOR
        fTable.Write(0, 0, ratio, 1/ratio, fCaptureRingBuffer[0]->ReadSpace(), fPlaybackRingBuffer[0]->WriteSpace());
    #endif

        // Reset all ringbuffers in case of failure
        if (failure) {
            jack_error("JackAudioAdapterInterface::PushAndPull ringbuffer failure... reset");
            ResetRingBuffers();
            return -1;
        } else {
            return 0;
        }
    }

    int JackAudioAdapterInterface::PullAndPush(float** inputBuffer, float** outputBuffer, unsigned int inNumberFrames) 
    {
        bool failure = false;
        fPullAndPushTime = GetMicroSeconds();
  
        // Push/pull from ringbuffer
        for (int i = 0; i < fCaptureChannels; i++) {
            if (fCaptureRingBuffer[i]->Read(inputBuffer[i], inNumberFrames) < inNumberFrames)
                failure = true;
        }

        for (int i = 0; i < fPlaybackChannels; i++) {
            if (fPlaybackRingBuffer[i]->Write(outputBuffer[i], inNumberFrames) < inNumberFrames)
                failure = true;
        }

        // Reset all ringbuffers in case of failure
        if (failure) {
            jack_error("JackCallbackAudioAdapter::PullAndPush ringbuffer failure... reset");
            Reset();
            return -1;
        } else {
            return 0;
        }
    }

} // namespace
