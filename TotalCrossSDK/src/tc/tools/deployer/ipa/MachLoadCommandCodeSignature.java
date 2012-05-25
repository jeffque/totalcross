package tc.tools.deployer.ipa;

import java.io.IOException;
import tc.tools.deployer.ipa.blob.BlobHandler;
import tc.tools.deployer.ipa.blob.EmbeddedSignature;

public class MachLoadCommandCodeSignature extends MachLoadCommand
{
   public EmbeddedSignature payload;
   public long blobFileOffset;
   public long blobFileSize;

   private int offset2FileSize;

   public void PatchPositionAndSize(ElephantMemoryWriter writer, long NewLength) throws IOException
   {
      this.blobFileSize = NewLength;
      writer.memorize();
      writer.moveTo(offset2FileSize);
      writer.writeUnsignedInt(this.blobFileSize);
      writer.moveBack();
   }

   protected void parseFromStream(ElephantMemoryReader reader) throws IOException, InstantiationException,
         IllegalAccessException
   {
      this.blobFileOffset = (int) reader.readUnsignedInt();
      this.offset2FileSize = reader.getPos();
      this.blobFileSize = (int) reader.readUnsignedInt();
      reader.memorize();
      reader.moveTo(this.blobFileOffset);
      reader.bStreamLittleEndian = false;
      this.payload = (EmbeddedSignature) BlobHandler.readFromStream(reader);
      reader.moveBack();
      reader.bStreamLittleEndian = true;
   }
}
